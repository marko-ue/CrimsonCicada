// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/RangedWeapons/RicochetRevolver/RW_RicochetRevolver.h"

void ARW_RicochetRevolver::BeginPlay()
{
	Super::BeginPlay();
}

void ARW_RicochetRevolver::PerformPrimaryAction()
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
