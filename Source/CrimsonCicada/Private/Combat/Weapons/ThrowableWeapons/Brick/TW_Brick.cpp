// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/ThrowableWeapons/Brick/TW_Brick.h"
#include "Camera/CameraComponent.h"

void ATW_Brick::BeginPlay()
{
	Super::BeginPlay();

	HandsRequired = 1;
}

void ATW_Brick::PerformPrimaryAction()
{
	// Launches the weapon in the direction of the camera's forward vector. 
	// Makes the weapon visible and enable physics.
	LaunchDirectionVector = CameraComp->GetForwardVector();
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->AddImpulse(LaunchDirectionVector * LaunchForceVector);
	WeaponMesh->AddTorqueInRadians(TorqueStrengthVector, NAME_None, true);
	
	// Function for handling thrown weapon (setting equipped weapon to none, detaching and making the weapon equippable)
	HandleWeaponThrown();
}