// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/RangedWeapons/ShotgunGun/RW_ShotgunGun.h"

void ARW_ShotgunGun::BeginPlay()
{
	Super::BeginPlay();
	
	PelletShootSceneComponent = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("SpreadLocation")));
}

void ARW_ShotgunGun::PerformPrimaryAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Scene component name: %s"), *PelletShootSceneComponent->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Scene component location: %s"), *PelletShootSceneComponent->GetRelativeLocation().ToString());
	
	FHitResult HitResultStraight;
	bool bHitStraight = PerformWeaponTraceComp->PerformStraightTraceFromCamera(Range, HitResultStraight, ECC_GameTraceChannel3);

	if (bHitStraight)
	{
		AActor* HitActorStraight = HitResultStraight.GetActor();
		if (HitActorStraight)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit actor with straight trace: %s"), *HitActorStraight->GetName());

			FHitResult HitResultSpread;
			bool bHitSpread = PerformWeaponTraceComp->PerformSpreadTraces(PelletShootSceneComponent->GetComponentLocation(), SpreadRange, HitResultSpread, ECC_GameTraceChannel3);
			
			if (bHitSpread)
			{
				AActor* HitActorSpread = HitResultSpread.GetActor();
				if (HitActorSpread)
				{
					UE_LOG(LogTemp, Warning, TEXT("Hit actor with spread trace: %s"), *HitActorSpread->GetName());
				}
			}
		}
	}
}
