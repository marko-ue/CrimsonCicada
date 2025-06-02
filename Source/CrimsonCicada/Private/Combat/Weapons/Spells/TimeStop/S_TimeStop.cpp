// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/Spells/TimeStop/S_TimeStop.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

void AS_TimeStop::BeginPlay()
{
	Super::BeginPlay();
	
	SpellCooldown = 1.0f;
	HandsRequired = 1;
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}
// make all enemies and entities stop moving but let player move and pre-throw throwables (stay in air until resumed)
void AS_TimeStop::CastSpell()
{
	if (!bIsSpellOnCooldown)
	{
		UE_LOG(LogTemp, Warning, TEXT("Casting time stop spell"));

		// Deal damage to the player while the time stop spell is active. 
		// Freeze all enemies and objects, only allow player movement.
		FDamageEvent TimeStopHealthDrainEvent;
		
		PlayerPawn->TakeDamage(10, TimeStopHealthDrainEvent, PlayerPawn->GetInstigatorController(), this);
		bIsSpellOnCooldown = true;
		

		FTimerHandle ResetCooldownHandle;
		GetWorldTimerManager().SetTimer(ResetCooldownHandle, [this]() { bIsSpellOnCooldown = false; }, SpellCooldown, false);
	}
}
