// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/Spells/TimeStop/S_TimeStop.h"

#include "Characters/Enemies/BaseEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/Damageable.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"


void AS_TimeStop::BeginPlay()
{
	Super::BeginPlay();
	
	SpellCooldown = 2.0f;
	HandsRequired = 1;
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void AS_TimeStop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("Is spell active: %d"), bIsSpellActive ? 1 : 0);
	UE_LOG(LogTemp, Warning, TEXT("Is spell on cooldown: %d"), bIsSpellOnCooldown ? 1 : 0);
}

void AS_TimeStop::SetEnemyMoveSpeed(bool bIsFrozen)
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UDamageable::StaticClass(), Enemies);

	for (AActor* Enemy : Enemies)
	{
		ACharacter* EnemyCharacter = Cast<ACharacter>(Enemy);
		if (EnemyCharacter)
		{
			ABaseEnemyCharacter* BaseEnemy = Cast<ABaseEnemyCharacter>(Enemy);
			if (BaseEnemy)
			{
				BaseEnemy->SetMovementSpeed(bIsFrozen);
			}
		}
	}
}

// when spell is inactive, activate it and start cooldown
// when spell becomes inactive, disable it and start the 2 second cooldodwn
// after the cooldown is done, allow activating the spell again
// make it so the cooldown isn't being set back to the 2 seconds if you try to use the spell when it's already on cooldown

// Make all enemies and entities stop moving but let player move and pre-throw throwables (stay in air until resumed)
void AS_TimeStop::CastSpell()
{
	if (!bIsSpellActive && !bIsSpellOnCooldown)
	{
		UE_LOG(LogTemp, Warning, TEXT("Casting time stop spell"));
		
		SetEnemyMoveSpeed(true);
		
		bIsSpellOnCooldown = true;
		bIsSpellActive = true;

		// Deal damage to the player while the time stop spell is active. 
		// Freeze all enemies and objects, only allow player movement.
		FDamageEvent TimeStopHealthDrainEvent;
		PlayerPawn->TakeDamage(10, TimeStopHealthDrainEvent, PlayerPawn->GetInstigatorController(), this);
		GetWorldTimerManager().SetTimer(DrainHealthHandle, [this, TimeStopHealthDrainEvent]() { PlayerPawn->TakeDamage(10, TimeStopHealthDrainEvent, PlayerPawn->GetInstigatorController(), this); }, 1.0f, true);
	}
	else
	{
		bIsSpellActive = false;

		if (!GetWorldTimerManager().IsTimerActive(ResetCooldownHandle))
		{
			GetWorldTimerManager().SetTimer(ResetCooldownHandle, [this]() { bIsSpellOnCooldown = false; }, SpellCooldown, false);
		}

		GetWorldTimerManager().ClearTimer(DrainHealthHandle);
		SetEnemyMoveSpeed(false);
	}
}
