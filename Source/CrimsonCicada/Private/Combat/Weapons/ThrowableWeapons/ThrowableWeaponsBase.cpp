// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/ThrowableWeapons/ThrowableWeaponsBase.h"

AThrowableWeaponsBase::AThrowableWeaponsBase()
{
}

void AThrowableWeaponsBase::BeginPlay()
{
	Super::BeginPlay();
}

void AThrowableWeaponsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AThrowableWeaponsBase::AddWeaponToInventory(EWeapon WeaponToAdd)
{
	
}

void AThrowableWeaponsBase::ClearThrowFlipbook()
{
	WeaponFlipbookComp->SetFlipbook(nullptr);
}

void AThrowableWeaponsBase::ApplyThrowPhysics()
{
	// Launches the weapon in the direction of the camera's forward vector.
	// Makes the weapon visible and enable physics.
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	LaunchDirectionVector = CameraComp->GetForwardVector();
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->AddImpulse(LaunchDirectionVector * LaunchForceVector);
	WeaponMesh->AddTorqueInRadians(TorqueStrengthVector, NAME_None, true);
	
	// Function for handling thrown weapon (setting equipped weapon to none, detaching and making the weapon equippable)
	HandleWeaponThrown();
}

void AThrowableWeaponsBase::PlayThrowFlipbook()
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

void AThrowableWeaponsBase::HandleWeaponThrown()
{
	// Makes the thrown weapon not equipped again, detaching, and making it equippable again
	InventoryComp->EquippedWeapon = nullptr;
	WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	bCanBeEquipped = false;
}
