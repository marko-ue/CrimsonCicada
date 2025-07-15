// Fill out your copyright notice in the Description page of Project Settings.
#include "Characters/Enemies/RubyCricket/CricketEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/BaseAIController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ACricketEnemyCharacter::ACricketEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_CricketObj(TEXT("/Game/AI/BT_Cricket.BT_Cricket"));
	if (BT_CricketObj.Succeeded())
	{
		CustomBehaviorTree = BT_CricketObj.Object;
	}
}

UBehaviorTree* ACricketEnemyCharacter::GetBehaviorTree() const
{
	return Super::GetBehaviorTree();
}

void ACricketEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (ABaseAIController* AIController = Cast<ABaseAIController>(GetController()))
	{
		
		AIController->DetectionRange = 500.0f;
		AIController->SetState(TEXT("Inactive"));
		if (AIController->BehaviorTreeAsset && AIController->GetBrainComponent() && !AIController->GetBrainComponent()->IsRunning())
		{
			AIController->RunBehaviorTree(AIController->BehaviorTreeAsset);
		}
	}
}

void ACricketEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ABaseAIController* AIController = Cast<ABaseAIController>(GetController());
	if (!AIController) return;

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard) return;

	AActor* Target = AIController->CurrentTarget;

	if (Target)
	{
		float Distance = GetDistanceTo(Target);
		bool bInLeapRange = Distance <= 900.0f;

		
		Blackboard->SetValueAsBool(TEXT("InLeapRange"), bInLeapRange);

		
		UE_LOG(LogTemp, Warning, TEXT("Distance to Target: %.2f | InLeapRange: %s"), Distance, bInLeapRange ? TEXT("TRUE") : TEXT("FALSE"));
	}
	else
	{
		
		Blackboard->SetValueAsBool(TEXT("InLeapRange"), false);
	}
	
	
	if (bIsLeaping && Target && GetDistanceTo(Target) <= 100.0f)
	{
		bHasHitTarget = true;
		LeapEnd();
	}
}


void ACricketEnemyCharacter::StartLeap()
{
	ABaseAIController* AIController = Cast<ABaseAIController>(GetController());
	if (!AIController) return;

	AActor* Target = AIController->CurrentTarget;
	FVector ToTarget = Target->GetActorLocation() - GetActorLocation();
	if (!bIsLeaping && LeapMontage)
	{
		
		bIsLeaping = true;
		bHasHitTarget = false;
		PlayAnimMontage(LeapMontage);
		FRotator LookAtRotation = FRotationMatrix::MakeFromX(ToTarget).Rotator();
		LookAtRotation.Pitch = 0.0f;
		LookAtRotation.Roll = 0.0f;
		SetActorRotation(LookAtRotation);

		FVector LaunchDirection = (ToTarget.GetSafeNormal() + FVector(0, 0, 0.2f)).GetSafeNormal();
		GetCharacterMovement()->AddImpulse(LaunchDirection * LeapForce, true);

		GetWorldTimerManager().SetTimer(LeapTimerHandle, this, &ACricketEnemyCharacter::LeapEnd, 1.0f, false);
	}
}

void ACricketEnemyCharacter::LeapEnd()
{
	bIsLeaping = false;
	if (!bHasHitTarget)
	{
		Destroy();
	}
	else if (bHasHitTarget)
	{
		ABaseAIController* AIController = Cast<ABaseAIController>(GetController());
		if (!AIController) return;
		UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
		if (!Blackboard) return;
		Blackboard->SetValueAsBool(TEXT("HasHitTarget"),true);
		
	}
}

void ACricketEnemyCharacter::Explosion()
{
	UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr,
		TArray<AActor*>(),this);
}


