// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/RangedWeapons/RangedWeaponsBase.h"

ARangedWeaponsBase::ARangedWeaponsBase()
{
}

void ARangedWeaponsBase::BeginPlay()
{
	Super::BeginPlay();

	PerformWeaponTraceComp = FindComponentByClass<UPerformWeaponTraceComponent>();
	
}

void ARangedWeaponsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARangedWeaponsBase::Reload(float InactivityDelay)
{
	if (AmmoInClip == ClipSize || Ammo == 0) { return; }

	PlayReloadFlipbook();

	bIsWeaponActive = true;

	int AmmoToReduce{ ClipSize - AmmoInClip };

	if (AmmoToReduce > Ammo)
	{
		AmmoInClip += Ammo;
		Ammo = 0;
	}
	else
	{
		Ammo -= AmmoToReduce;
		AmmoInClip = ClipSize;
	}

	UE_LOG(LogTemp, Warning, TEXT("Reloading"));
	UE_LOG(LogTemp, Warning, TEXT("Ammo left: %i"), Ammo);
	UE_LOG(LogTemp, Warning, TEXT("Ammo in clip: %i"), AmmoInClip);

	//UE_LOG(LogTemp, Warning, TEXT("Timer delay: %f"), InactivityDelay);

	GetWorld()->GetTimerManager().SetTimer(SetWeaponInactiveTimerHandle, this, &AAllWeaponsBase::SetWeaponInactive, InactivityDelay, false);
}

void ARangedWeaponsBase::StartAutomaticFire()
{
}

void ARangedWeaponsBase::StopAutomaticFire()
{
}

void ARangedWeaponsBase::PerformPrimaryActionAutomatic()
{
}

// If the ranged weapon base specific flipbooks are not nullptr, get their length
void ARangedWeaponsBase::GetFlipbookLengthIfValid()
{
	if (ShootFlipbook)
	{
		ShootFlipbookLength = ShootFlipbook->GetTotalDuration();
	}

	if (ReloadFlipbook)
	{
		ReloadFlipbookLength = ReloadFlipbook->GetTotalDuration();
	}
}

void ARangedWeaponsBase::PlayShootFlipbook(float InactivityDelay)
{
	if (!bIsWeaponActive)
	{
		WeaponFlipbookComp->SetFlipbook(ShootFlipbook);
		WeaponFlipbookComp->PlayFromStart();
		// Check if dual wield spell is active
		if (!bIsDualWieldSpellActive)
		{
			bIsWeaponActive = true;

			//UE_LOG(LogTemp, Warning, TEXT("Timer delay: %f"), InactivityDelay);
			
			GetWorld()->GetTimerManager().SetTimer(SetWeaponInactiveTimerHandle, this, &AAllWeaponsBase::SetWeaponInactive, InactivityDelay, false);
		}
	}

	if (bIsDualWieldSpellActive)
	{
		if (!bIsWeaponActive)
		{
			WeaponDuelWieldFlipbookComp->SetFlipbook(ShootFlipbook);
			WeaponDuelWieldFlipbookComp->PlayFromStart();
			bIsWeaponActive = true;

			//UE_LOG(LogTemp, Warning, TEXT("Timer delay: %f"), InactivityDelay);
		
			GetWorld()->GetTimerManager().SetTimer(SetWeaponInactiveTimerHandle, this, &AAllWeaponsBase::SetWeaponInactive, InactivityDelay, false);
		}
	}
}

void ARangedWeaponsBase::PlayReloadFlipbook()
{
	if (!bIsWeaponActive)
	{
		WeaponFlipbookComp->SetFlipbook(ReloadFlipbook);
		WeaponFlipbookComp->PlayFromStart();
	}
}

