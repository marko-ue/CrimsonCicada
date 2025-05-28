// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/Spells/TimeStop/S_TimeStop.h"

void AS_TimeStop::BeginPlay()
{
	Super::BeginPlay();
	
	SpellCooldown = 5.0f;
	HandsRequired = 1;
}

void AS_TimeStop::CastSpell()
{
	UE_LOG(LogTemp, Warning, TEXT("Casting Spell"));
	
	if (!bIsSpellOnCooldown)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast time stop spell"));
		bIsSpellOnCooldown = true;

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this]() { bIsSpellOnCooldown = false; }, SpellCooldown, false);
	}
}
