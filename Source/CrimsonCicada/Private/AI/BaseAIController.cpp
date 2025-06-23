
#include "AI/BaseAIController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/BaseEnemyCharacter.h"

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
	if (!ControlledPawn) 
	{
		UE_LOG(LogTemp, Warning, TEXT("No Controlled Pawn!"));
		return;
	}
    
	if (CurrentTarget)
	{
		float Distance = ControlledPawn->GetDistanceTo(CurrentTarget);
		bool bInMeleeRange = (Distance <= MeleeRange);
       
		if (bInMeleeRange)
		{
			ABaseEnemyCharacter* EnemyChar = Cast<ABaseEnemyCharacter>(ControlledPawn);
			if (EnemyChar)
			{
				EnemyChar->PlayAttackMontage();
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Target: %s, Distance: %f, MeleeRange: %f, CanAttack: %s"), 
			   *CurrentTarget->GetName(), Distance, MeleeRange, bInMeleeRange ? TEXT("TRUE") : TEXT("FALSE"));
       
		if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsBool(TEXT("CanAttack"), bInMeleeRange);
			UE_LOG(LogTemp, Warning, TEXT("Successfully set CanAttack to: %s"), bInMeleeRange ? TEXT("TRUE") : TEXT("FALSE"));
		}	
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No Blackboard Component in Tick!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No CurrentTarget - setting CanAttack to false"));
		if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsBool(TEXT("CanAttack"), false);
		}
	}
	
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();
	if (BehaviorTreeAsset)
	{
		if (UseBlackboard(BehaviorTreeAsset->GetBlackboardAsset(),BlackboardComponent))
		{
			RunBehaviorTree(BehaviorTreeAsset);
		};
	}
}

void ABaseAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("=== Perception Debug ==="));
	UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), Actor ? *Actor->GetName() : TEXT("NULL"));
	UE_LOG(LogTemp, Warning, TEXT("Actor Pointer: %p"), Actor);
	UE_LOG(LogTemp, Warning, TEXT("Successfully Sensed: %s"), Stimulus.WasSuccessfullySensed() ? TEXT("TRUE") : TEXT("FALSE"));
	UE_LOG(LogTemp, Warning, TEXT("Current Target Before: %s"), CurrentTarget ? *CurrentTarget->GetName() : TEXT("NULL"));
    
	if (Stimulus.WasSuccessfullySensed())
	{
		CurrentTarget = Actor;
		if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor);
			UE_LOG(LogTemp, Warning, TEXT("Blackboard TargetActor set to: %s"), *Actor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No Blackboard Component found!"));
		}
        
		UE_LOG(LogTemp, Warning, TEXT("Target Perceived: %s"), *Actor->GetName());
	}
	else
	{
		if (CurrentTarget == Actor)
		{
			CurrentTarget = nullptr;
			if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
			{
				BlackboardComp->SetValueAsObject(TEXT("TargetActor"), nullptr);
				UE_LOG(LogTemp, Warning, TEXT("Blackboard TargetActor cleared"));
			}
            
			UE_LOG(LogTemp, Warning, TEXT("Lost sight of: %s"), *Actor->GetName());
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("=== End Debug ==="));
};

