// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/RangedWeapons/Shotgun/RW_Shotgun.h"

void ARW_Shotgun::BeginPlay()
{
	Super::BeginPlay();

	BulletShootPoint = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("BulletShootPoint")));
	HandsRequired = 2;

	GetFlipbookLengthIfValid();

	WeaponFlipbookComp->OnFinishedPlaying.AddDynamic(this, &AAllWeaponsBase::SetWeaponInactive);

	IdleFlipbook = IdleFullFlipbook;
	WalkFlipbook = WalkFullFlipbook;
	RunFlipbook = RunFullFlipbook;

	UnequipTimerDelay = 0.4f;
}

void ARW_Shotgun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AmmoInClip <= 0)
	{
		EquipFlipbook = EquipEmptyFlipbook;
	}
	else
	{
		EquipFlipbook = EquipFullFlipbook;
	}
}

void ARW_Shotgun::PerformPrimaryAction()
{
	if (bIsWeaponActive) { return; }

	if (AmmoInClip <= 0) { return; }

	IdleFlipbook = IdleEmptyFlipbook;
	WalkFlipbook = WalkEmptyFlipbook;
	RunFlipbook = RunEmptyFlipbook;

	PlayShootFlipbook(ShootFlipbookLength);

	ReduceAmmoInClipByAmount(1);
	
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

void ARW_Shotgun::Reload(float InactivityDelay)
{
	Super::Reload(ReloadFlipbookLength);

	if (Ammo > 0)
	{
		IdleFlipbook = IdleFullFlipbook;
		WalkFlipbook = WalkFullFlipbook;
		RunFlipbook = RunFullFlipbook;
	}
}

void ARW_Shotgun::PlayShootFlipbook(float InactivityDelay)
{
	Super::PlayShootFlipbook(ShootFlipbookLength);
}

void ARW_Shotgun::PlayReloadFlipbook()
{
	Super::PlayReloadFlipbook();
}
