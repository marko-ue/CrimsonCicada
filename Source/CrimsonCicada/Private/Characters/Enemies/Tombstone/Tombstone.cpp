// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Tombstone/Tombstone.h"

#include "AkAudioDeviceShareSet.h"
#include "AI/Tombstone/TombstoneAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
		bool bInAttackRange = Distance >= MinimumAttackRange && Distance <= MaxAttackRange;
		if (Distance < MinimumAttackRange)
		{
			PushTargetAway(Target);
			return; 
		}
		
		if (bInAttackRange)
		{
			
			TombstoneAiController->SetState("Attacking");
			
		}
		else
		{
			
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
	if (Target && GetDistanceTo(Target) <= MinimumAttackRange)
	{
		bIsAttacking = true;
		if (SwingMontage && GetMesh() && GetMesh()->GetAnimInstance())
		{
			
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

			AnimInstance->Montage_Stop(0.1f);
			AnimInstance->Montage_Play(SwingMontage, 1.0f);
		}
	}
	else
	{
		bIsAttacking = false;
	}
}

void ATombstone::PushTargetAway(AActor* Target)
{
	FVector Direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FVector PushLocation = GetActorLocation() + Direction * MaxAttackRange;
	ACharacter* TargetCharacter = Cast<ACharacter>(GetController());
	ATombstoneAIController* TombstoneController = Cast<ATombstoneAIController>(GetController());
	UBlackboardComponent* Blackboard = TombstoneController->GetBlackboardComponent();
	float Distance = GetDistanceTo(Target);
	if (TargetCharacter && Distance <= AttackRange)
	{
		Blackboard->SetValueAsBool(TEXT("IsTooClose"), true);
		FVector PushForce = Direction * 60.0f + FVector(100.0f, 0.0f ,0.0f);
		TargetCharacter->LaunchCharacter(PushForce, true ,true);
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Stop(0.1f);
		AnimInstance->Montage_Play(PushMontage , 1.0f);
		
	}
	else
	{
		Blackboard->SetValueAsBool(TEXT("IsTooClose"), false);
		Target->SetActorLocation(PushLocation);
		
	}
}


void ATombstone::DealDamage(float DamageAmount)
{
	FDamageEvent DamageEvent;
	TakeDamage(DamageAmount, DamageEvent, PlayerPawn->GetController(), this);
}

