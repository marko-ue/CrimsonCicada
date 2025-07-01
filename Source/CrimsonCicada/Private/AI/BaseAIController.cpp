
#include "AI/BaseAIController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/BaseEnemyCharacter.h"
#include "Characters/Enemies/RubyCricket/CricketEnemyCharacter.h"

ABaseAIController::ABaseAIController()
{
	//Create perception component
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SetPerceptionComponent(*PerceptionComponent);

	//Create and configure sight sense
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 1700.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->SetMaxAge(5.0f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 0.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	//Create and configure hearing sense
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 1200.0f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	//Add sense configs to perception component
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->ConfigureSense(*HearingConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	//Bind perception update event
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnPerceptionUpdated);
	
}

void ABaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	if (CurrentTarget)
	{
		float Distance = ControlledPawn->GetDistanceTo(CurrentTarget);
		bool bInDetectionRange = (Distance <= DetectionRange);
		bool bInEngagementRange = (Distance <= EngagementRange);

		if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsBool(TEXT("InDetectionRange"), bInDetectionRange);
			BlackboardComp->SetValueAsBool(TEXT("InEngagementRange"), bInEngagementRange);
			BlackboardComp->SetValueAsObject(TEXT("TargetActor"), CurrentTarget);
			BlackboardComp->SetValueAsName(TEXT("CurrentState"), CurrentState);
		}

		if (bInEngagementRange)
		{
			
		}
	}
	
		
	else if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsBool(TEXT("InDetectionRange"), false);
		BlackboardComp->SetValueAsBool(TEXT("InEngagementRange"), false);
		BlackboardComp->SetValueAsObject(TEXT("TargetActor"), nullptr);
		
		
	}
	
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || Actor == GetPawn() || Actor->ActorHasTag("Enemy"))
		return;
	if (Stimulus.WasSuccessfullySensed())
	{
		CurrentTarget = Actor;
		if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor);
			if (CurrentState == TEXT("Inactive" || CurrentState == TEXT("Territorial")))
			{
				SetState(TEXT("Triggered"));
			}
		}
	}
	else if (CurrentTarget == Actor)
	{
		CurrentTarget = nullptr;
		if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsObject(TEXT("TargetActor"), nullptr);
			if (CurrentState == TEXT("Triggered") && GetPawn()->GetDistanceTo(Actor) > DetectionRange)
			{
				SetState(TEXT("Territorial"));
			}
			
		}
	}
	
	
}

void ABaseAIController::SetState(FName NewState)
{
	CurrentState = NewState;
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsName(TEXT("CurrentState"), NewState);
	}
	ACricketEnemyCharacter* Cricket = Cast<ACricketEnemyCharacter>(GetPawn());
	if (Cricket)
	{
		if (NewState == TEXT("Inactive"))
			Cricket->CurrentState = ECricketState::Inactive;
		else if (NewState == TEXT("Territorial"))
			Cricket->CurrentState = ECricketState::Territorial;
		else if (NewState == TEXT("Triggered"))
			Cricket->CurrentState = ECricketState::Triggered;
	}
}



void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (!InPawn) return ;
	ABaseEnemyCharacter* ControlledPawn = Cast<ABaseEnemyCharacter>(InPawn);
	if (ControlledPawn)
	{
		UBehaviorTree* BT = ControlledPawn->GetBehaviorTree();
		if (BT)
		{
			BehaviorTreeAsset = BT;
			if (UseBlackboard(BT->GetBlackboardAsset(),BlackboardComponent))
			{
				RunBehaviorTree(BT);
				if (BlackboardComponent)
				{
					BlackboardComponent->SetValueAsName(TEXT("CurrentState"), TEXT("Inactive"));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ENEMY HAS NO BEHAVIOR TREE"))
		}
	}
	
};

