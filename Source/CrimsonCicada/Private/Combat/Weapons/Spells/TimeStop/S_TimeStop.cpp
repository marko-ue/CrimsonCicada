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

void AS_TimeStop::CastSpell()
{
	if (!bIsSpellOnCooldown)
	{
		UE_LOG(LogTemp, Warning, TEXT("Casting time stop spell"));

		FDamageEvent TimeStopHealthDrainEvent;
		
		PlayerPawn->TakeDamage(10, TimeStopHealthDrainEvent, PlayerPawn->GetInstigatorController(), this);
		bIsSpellOnCooldown = true;
		

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this]() { bIsSpellOnCooldown = false; }, SpellCooldown, false);
	}
}
