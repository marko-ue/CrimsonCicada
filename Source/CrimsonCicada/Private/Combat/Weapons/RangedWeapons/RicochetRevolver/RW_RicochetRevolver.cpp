// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/RangedWeapons/RicochetRevolver/RW_RicochetRevolver.h"

void ARW_RicochetRevolver::BeginPlay()
{
	Super::BeginPlay();
	BulletShootPoint = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("BulletShootPoint")));
	HandsRequired = 2;

	GetFlipbookLengthIfValid();

	WeaponFlipbookComp->OnFinishedPlaying.AddDynamic(this, &AAllWeaponsBase::SetWeaponInactive);
	WeaponDuelWieldFlipbookComp->OnFinishedPlaying.AddDynamic(this, &AAllWeaponsBase::SetWeaponInactive);
}

void ARW_RicochetRevolver::PerformPrimaryAction()
{
	if (bIsWeaponActive) { return; }

	if (AmmoInClip <= 0) { return; }

	PlayShootFlipbook(ShootFlipbookLength);

	ReduceAmmoInClipByAmount(1);
				
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

void ARW_RicochetRevolver::Reload(float InactivityDelay)
{
	Super::Reload(ReloadFlipbookLength);
}

void ARW_RicochetRevolver::PlayShootFlipbook(float InactivityDelay)
{
	Super::PlayShootFlipbook(ShootFlipbookLength);
}

void ARW_RicochetRevolver::PlayReloadFlipbook()
{
	Super::PlayReloadFlipbook();
}
