// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/Spells/DualWield/S_DualWield.h"

void AS_DualWield::BeginPlay()
{
	Super::BeginPlay();

	SpellCooldown = 60.0f;
	HandsRequired = 1;
	SpellDuration = 15.0f;

	InventoryComp = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UInventoryComponent>();
}

void AS_DualWield::CastSpell()
{
	if (!bIsSpellOnCooldown)
	{
		if (InventoryComp->EquippedWeapon)
		{
			if (InventoryComp->EquippedWeapon->HandsRequired == 1)
			{
				InventoryComp->EquippedWeapon->bIsDualWieldSpellActive = true;
				
				UE_LOG(LogTemp, Warning, TEXT("Dual wield active"));
		
				bIsSpellOnCooldown = true;

				FTimerHandle EndDualWieldSpell;
				GetWorldTimerManager().SetTimer(EndDualWieldSpell, [this]() { bIsDualWieldSpellActive = false; }, SpellDuration, false);
	
				FTimerHandle ResetCooldownHandle;
				GetWorldTimerManager().SetTimer(ResetCooldownHandle, [this]() { bIsSpellOnCooldown = false; }, SpellCooldown, false);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Cannot activate duel wield spell on two-handed weapons"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot activate duel wield spell if there is not a weapon equipped"));
		}
	}
}
