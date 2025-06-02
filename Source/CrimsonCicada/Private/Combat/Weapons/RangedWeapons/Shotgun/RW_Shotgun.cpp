// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/RangedWeapons/Shotgun/RW_Shotgun.h"

void ARW_Shotgun::BeginPlay()
{
	Super::BeginPlay();

	BulletShootPoint = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("BulletShootPoint")));
	HandsRequired = 2;
}

void ARW_Shotgun::PerformPrimaryAction()
{
	// The shotgun weapon spawns a projectile into the world and calls is primary action function (which in this case gets launched, like a throwable, not a bullet)
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	
	ATW_ShotgunGun* SpawnedShotgunGun = GetWorld()->SpawnActor<ATW_ShotgunGun>(ProjectileClass, BulletShootPoint->GetComponentLocation(), BulletShootPoint->GetComponentRotation(), SpawnParams);

	if (SpawnedShotgunGun)
	{
		SpawnedShotgunGun->PerformPrimaryAction();
	}
}
