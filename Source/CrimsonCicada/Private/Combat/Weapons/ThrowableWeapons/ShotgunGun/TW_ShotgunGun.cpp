// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/ThrowableWeapons/ShotgunGun/TW_ShotgunGun.h"
#include "Camera/CameraComponent.h"

void ATW_ShotgunGun::BeginPlay()
{
	Super::BeginPlay();
	
	LaunchForceVector = FVector(200,200,425);
	TorqueStrengthVector = FVector(200.f, 125.f, 500.f);
}

void ATW_ShotgunGun::PerformPrimaryAction()
{
	LaunchDirectionVector = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UCameraComponent>()->GetForwardVector();
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->AddImpulse(LaunchDirectionVector * LaunchForceVector);
	WeaponMesh->AddTorqueInRadians(TorqueStrengthVector, NAME_None, true);
	
	HandleWeaponThrown();
}
