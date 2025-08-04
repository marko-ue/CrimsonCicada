// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Tombstone/Tombstone.h"

#include "AkAudioDeviceShareSet.h"
#include "AI/Tombstone/TombstoneAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Engine/DamageEvents.h"

ATombstone::ATombstone()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_TombstoneObj(TEXT("/Game/AI/BT/Tombstone.BT_Tombstone"));
	if (BT_TombstoneObj.Succeeded())
	{
		CustomBehaviorTree = BT_TombstoneObj.Object;
	}
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
}

void ATombstone::BeginPlay()
{
	Super::BeginPlay();
}

void ATombstone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ATombstoneAIController* TombstoneAiController = Cast<ATombstoneAIController>(GetController());
	if (!TombstoneAiController) return;
	UBlackboardComponent* Blackboard = TombstoneAiController->GetBlackboardComponent();
	if (!Blackboard) return;
	AActor* Target = TombstoneAiController->CurrentTarget;
	if (Target)
	{
		float Distance = GetDistanceTo(Target);
		bool bInAttackRange = Distance <= 250.0f;
		if (bInAttackRange)
		{
			
			TombstoneAiController->SetState("Attacking");
			
		}
		else
		{
			Blackboard->SetValueAsBool("IsAttacking", false);
			TombstoneAiController->SetState("Chasing");
		}
	}
	else
	{
		TombstoneAiController->SetState("Idle");
	}
	
}

UBehaviorTree* ATombstone::GetBehaviorTree() const
{
	return Super::GetBehaviorTree();
}

void ATombstone::StartAttack()
{
	ATombstoneAIController* TombstoneAiController = Cast<ATombstoneAIController>(GetController());
	if (!TombstoneAiController) return;
	AActor* Target = TombstoneAiController->CurrentTarget;
	if (Target && GetDistanceTo(Target) <= 250.0f)
	{
		bIsAttacking = true;
		PlayAnimMontage(SwingMontage);
	}
	else
	{
		bIsAttacking = false;
	}
}

void ATombstone::DealDamage(float DamageAmount)
{
	FDamageEvent DamageEvent;
	TakeDamage(DamageAmount, DamageEvent, PlayerPawn->GetController(), this);
}

