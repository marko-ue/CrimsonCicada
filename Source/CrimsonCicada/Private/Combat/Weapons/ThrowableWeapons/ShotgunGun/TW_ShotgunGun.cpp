// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/ThrowableWeapons/ShotgunGun/TW_ShotgunGun.h"
#include "Camera/CameraComponent.h"
#include "Combat/Weapons/Components/PerformWeaponTraceComponent.h"

void ATW_ShotgunGun::BeginPlay()
{
	Super::BeginPlay();

	BulletShootPoint = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("BulletShootPoint")));
	PerformWeaponTraceComp = FindComponentByClass<UPerformWeaponTraceComponent>();
	
	LaunchForceVector = FVector(200,200,425);
	TorqueStrengthVector = FVector(200.f, 125.f, 500.f);
}

void ATW_ShotgunGun::PerformPrimaryAction()
{
	LaunchDirectionVector = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UCameraComponent>()->GetForwardVector();
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->AddImpulse(LaunchDirectionVector * LaunchForceVector);
	WeaponMesh->AddTorqueInRadians(TorqueStrengthVector, NAME_None, true);
	
	//HandleWeaponThrown();

	// Timer that will allow collisions to be checked by making a bool true, bool checked in blueprint
	FTimerHandle SetCollisionsHandle;
	FTimerDelegate SetCollisionsDelegate = FTimerDelegate::CreateUObject(this, &ATW_ShotgunGun::SetShouldCheckForCollisions, true);
	GetWorldTimerManager().SetTimer(SetCollisionsHandle, SetCollisionsDelegate, 0.1, false);
}

void ATW_ShotgunGun::SetShouldCheckForCollisions(bool ShouldCheckForCollisions)
{
	bShouldCheckForCollisions = ShouldCheckForCollisions;
}

void ATW_ShotgunGun::Fire()
{
	TArray<FHitResult> HitResultsSpread;
	if (PerformWeaponTraceComp->PerformSpreadTraces(BulletShootPoint->GetComponentLocation(), BulletShootPoint->GetForwardVector(), SpreadAngleDegrees,  NumberOfTraces, SpreadRange, HitResultsSpread, ECC_GameTraceChannel3))
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), HitResultsSpread.Num());
	}
}

void ATW_ShotgunGun::OnFired()
{
	Destroy();
}
