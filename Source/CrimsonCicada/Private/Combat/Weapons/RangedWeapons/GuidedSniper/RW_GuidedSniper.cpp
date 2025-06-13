// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/RangedWeapons/GuidedSniper/RW_GuidedSniper.h"

void ARW_GuidedSniper::BeginPlay()
{
	Super::BeginPlay();

	HandsRequired = 2;
}

void ARW_GuidedSniper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARW_GuidedSniper::PerformPrimaryAction()
{
	if (bIsWeaponActive) { return; }
	
	// The guided sniper weapon spawns its projectile into the world and calls its launch function (which in this case shoots it from the camera forward vector)
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	
	AP_SniperBullet* SpawnedSniperBullet = GetWorld()->SpawnActor<AP_SniperBullet>(ProjectileClass, CameraComp->GetComponentLocation(), CameraComp->GetComponentRotation(), SpawnParams);

	if (SpawnedSniperBullet)
	{
		SpawnedSniperBullet->LaunchProjectile();

		// Switch camera to the bullet's
		GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(SpawnedSniperBullet, 0.3f);
	}

	bIsWeaponActive = true;
}

void ARW_GuidedSniper::PlayShootFlipbook()
{
	Super::PlayShootFlipbook();
}
