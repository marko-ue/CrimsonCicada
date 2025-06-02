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

void AThrowableWeaponsBase::HandleWeaponThrown()
{
	// Makes the thrown weapon not equipped again, detaching, and making it equippable again
	InventoryComp->EquippedWeapon = nullptr;
	WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	bCanBeEquipped = false;
}