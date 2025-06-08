// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/ThrowableWeapons/Brick/TW_Brick.h"
#include "Camera/CameraComponent.h"


void ATW_Brick::BeginPlay()
{
	Super::BeginPlay();

	HandsRequired = 1;
	
	GetFlipbookLengthIfValid();
}

void ATW_Brick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

void ATW_Brick::PerformPrimaryAction()
{
	// Launches the weapon in the direction of the camera's forward vector. 
	// Makes the weapon visible and enable physics.
	if (bIsWeaponActive) { return; }
	
	PlayThrowFlipbook();
	
	
}

void ATW_Brick::PlayThrowFlipbook()
{
	// Plays the shoot flipbook if not already playing/weapon active
	if (!bIsWeaponActive)
	{
		WeaponFlipbookComp->SetFlipbook(ThrowFlipbook);
		WeaponFlipbookComp->PlayFromStart();
		// Check if dual wield spell is active
		if (!bIsDualWieldSpellActive)
		{
			bIsWeaponActive = true;
		}
	}
	
	// Makes the weapon inactive after the duration of the flipbook and some extra to avoid bugs
	GetWorld()->GetTimerManager().SetTimer(SetWeaponInactiveTimerHandle, this, &AAllWeaponsBase::SetWeaponInactive, ThrowFlipbookLength + 0.25f, false);
	GetWorld()->GetTimerManager().SetTimer(RemoveFlipbookTimerHandle, this, &AThrowableWeaponsBase::ClearThrowFlipbook, ThrowFlipbookLength + 0.55f, false);
	GetWorld()->GetTimerManager().SetTimer(ApplyPhysicsTimerHandle, this, &AThrowableWeaponsBase::ApplyThrowPhysics, ThrowFlipbookLength + 0.32f, false);
}
