// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/ThrowableWeapons/Brick/TW_Brick.h"
#include "Camera/CameraComponent.h"

void ATW_Brick::BeginPlay()
{
	Super::BeginPlay();

	LaunchForceVector = FVector(200,200,425);
	TorqueStrengthVector = FVector(200.f, 125.f, 500.f);
}

void ATW_Brick::PerformPrimaryAction()
{
	LaunchDirectionVector = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UCameraComponent>()->GetForwardVector();
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->AddImpulse(LaunchDirectionVector * LaunchForceVector);
	WeaponMesh->AddTorqueInRadians(TorqueStrengthVector, NAME_None, true);
	
	HandleWeaponThrown();
}
