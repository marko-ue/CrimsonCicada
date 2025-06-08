// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/RangedWeapons/RicochetRevolver/RW_RicochetRevolver.h"

void ARW_RicochetRevolver::BeginPlay()
{
	Super::BeginPlay();
	BulletShootPoint = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("BulletShootPoint")));
	HandsRequired = 2;
}

void ARW_RicochetRevolver::PerformPrimaryAction()
{
	
	// The ricochet revolver weapon spawns its projectile into the world and calls its launch function (which in this case shoots it from the camera forward vector)
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	
	AP_RicochetBullet* SpawnedShotgunGun = GetWorld()->SpawnActor<AP_RicochetBullet>(ProjectileClass, CameraComp->GetComponentLocation(), CameraComp->GetComponentRotation(), SpawnParams);

	if (SpawnedShotgunGun)
	{
		SpawnedShotgunGun->LaunchProjectile();
	}
}
