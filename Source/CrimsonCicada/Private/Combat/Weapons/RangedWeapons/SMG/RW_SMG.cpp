// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/RangedWeapons/SMG/RW_SMG.h"

void ARW_SMG::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

void ARW_SMG::BeginPlay()
{
	Super::BeginPlay();

	bIsAutomatic = true;
	HandsRequired = 2;
}

void ARW_SMG::PerformPrimaryActionAutomatic()
{
	// Performs a regular trace from the camera but automatic
	FHitResult HitResultStraight;
	bool bHit = PerformWeaponTraceComp->PerformStraightTraceFromCamera(Range, HitResultStraight, ECC_GameTraceChannel3);

	if (bHit)
	{
		AActor* HitActor = HitResultStraight.GetActor();
		if (HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit actor with straight trace: %s"), *HitActor->GetName());
		}
	}
}

void ARW_SMG::StartAutomaticFire()
{
	// Allows the player to hold down the input and have the weapon shoot automtically through the timer
	if (!GetWorld()->GetTimerManager().IsTimerActive(AutomaticFireTimerHandle))
	{
		PerformPrimaryActionAutomatic();
		
		GetWorld()->GetTimerManager().SetTimer(
			AutomaticFireTimerHandle,
			this,
			&ARW_SMG::PerformPrimaryActionAutomatic,
			FireRate,
			true
		);
	}
}

void ARW_SMG::StopAutomaticFire()
{
	// Stops the looping timer for shooting when input released
	GetWorld()->GetTimerManager().ClearTimer(AutomaticFireTimerHandle);
}
