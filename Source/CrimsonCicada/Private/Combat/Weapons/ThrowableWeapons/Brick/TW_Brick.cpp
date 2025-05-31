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
	LaunchDirectionVector = CameraComp->GetForwardVector();
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->AddImpulse(LaunchDirectionVector * LaunchForceVector);
	WeaponMesh->AddTorqueInRadians(TorqueStrengthVector, NAME_None, true);
	
	HandleWeaponThrown();
}