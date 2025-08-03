// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Tombstone/Tombstone.h"
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
}

UBehaviorTree* ATombstone::GetBehaviorTree() const
{
	return Super::GetBehaviorTree();
}

void ATombstone::DealDamage(float DamageAmount)
{
	FDamageEvent DamageEvent;
	TakeDamage(DamageAmount, DamageEvent, PlayerPawn->GetController(), this);
}

