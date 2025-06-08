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
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	LaunchDirectionVector = CameraComp->GetForwardVector();
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->AddImpulse(LaunchDirectionVector * LaunchForceVector);
	WeaponMesh->AddTorqueInRadians(TorqueStrengthVector, NAME_None, true);
	
	// Function for handling thrown weapon (setting equipped weapon to none, detaching and making the weapon equippable)
	HandleWeaponThrown();
}

void AThrowableWeaponsBase::HandleWeaponThrown()
{
	// Makes the thrown weapon not equipped again, detaching, and making it equippable again
	InventoryComp->EquippedWeapon = nullptr;
	WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	bCanBeEquipped = false;
}
