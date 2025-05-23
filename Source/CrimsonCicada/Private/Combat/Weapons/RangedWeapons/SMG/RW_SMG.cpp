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
}

void ARW_SMG::PerformPrimaryActionAutomatic()
{
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
	GetWorld()->GetTimerManager().ClearTimer(AutomaticFireTimerHandle);
}
