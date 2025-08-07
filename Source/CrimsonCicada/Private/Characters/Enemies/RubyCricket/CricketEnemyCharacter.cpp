// Fill out your copyright notice in the Description page of Project Settings.
#include "Characters/Enemies/RubyCricket/CricketEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Cricket/CricketAIController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DamageEvents.h"

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
	
	if (ACricketAIController* CricketAiController = Cast<ACricketAIController>(GetController()))
	{
		
		CricketAiController->DetectionRange = 500.0f;
		CricketAiController->SetState(TEXT("Inactive"));
		if (CricketAiController->BehaviorTreeAsset && CricketAiController->GetBrainComponent() && !CricketAiController->GetBrainComponent()->IsRunning())
		{
			CricketAiController->RunBehaviorTree(CricketAiController->BehaviorTreeAsset);
		}
	}
}

void ACricketEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACricketAIController* CricketAiController = Cast<ACricketAIController>(GetController());
	if (!CricketAiController) return;

	UBlackboardComponent* Blackboard = CricketAiController->GetBlackboardComponent();
	if (!Blackboard) return;

	AActor* Target = CricketAiController->CurrentTarget;

	if (Target)
	{
		float Distance = GetDistanceTo(Target);
		bool bInLeapRange = Distance <= 600.0f;

		
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
	ACricketAIController* CricketAiController = Cast<ACricketAIController>(GetController());
	if (!CricketAiController) return;

	AActor* Target = CricketAiController->CurrentTarget;
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
		ACricketAIController* CricketAiController = Cast<ACricketAIController>(GetController());
		if (!CricketAiController) return;
		UBlackboardComponent* Blackboard = CricketAiController->GetBlackboardComponent();
		if (!Blackboard) return;
		Blackboard->SetValueAsBool(TEXT("HasHitTarget"),true);
		
	}
}

void ACricketEnemyCharacter::Explosion()
{
	UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr,
		TArray<AActor*>(),this);
}

void ACricketEnemyCharacter::DealDamage(float DamageAmount)
{
	FDamageEvent DamageEvent;
	
	TakeDamage(DamageAmount, DamageEvent, PlayerPawn->GetController(), this);
}


