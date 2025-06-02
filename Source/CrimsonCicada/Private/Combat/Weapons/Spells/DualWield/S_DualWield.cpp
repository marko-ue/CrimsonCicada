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
			// The spell will only work on weapons that are one handed. Two handed weapons cannot be dual wielded
			if (InventoryComp->EquippedWeapon->HandsRequired == 1)
			{
				// Sets the dual wield spell to be active on the equipped weapon, for the purpose of other checks
				InventoryComp->EquippedWeapon->bIsDualWieldSpellActive = true;
				
				UE_LOG(LogTemp, Warning, TEXT("Dual wield active"));
		
				bIsSpellOnCooldown = true;

				// After the spell duration ends, remove the effect. Allow the player to activate the spell again after it goes off cooldown.
				FTimerHandle EndDualWieldSpell;
				GetWorldTimerManager().SetTimer(EndDualWieldSpell, [this]() { InventoryComp->EquippedWeapon->bIsDualWieldSpellActive = false; }, SpellDuration, false);
	
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
