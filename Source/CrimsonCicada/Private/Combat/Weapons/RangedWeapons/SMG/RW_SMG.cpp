// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/RangedWeapons/SMG/RW_SMG.h"

#include "Characters/MainCharacter/CicadaMainCharacter.h"

void ARW_SMG::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

void ARW_SMG::BeginPlay()
{
	Super::BeginPlay();

	bIsAutomatic = true;
	HandsRequired = 2;

	GetFlipbookLengthIfValid();

	WeaponFlipbookComp->OnFinishedPlaying.AddDynamic(this, &AAllWeaponsBase::SetWeaponInactive);
	WeaponDuelWieldFlipbookComp->OnFinishedPlaying.AddDynamic(this, &AAllWeaponsBase::SetWeaponInactive);
}

void ARW_SMG::PerformPrimaryActionAutomatic()
{
	if (AmmoInClip <= 0) { return; }
	
	PlayShootFlipbook(ShootFlipbookLength + 0.25f);
	
	ReduceAmmoInClipByAmount(1);
	
	// Performs a regular trace from the camera but automatic
	FHitResult HitResultStraight;
	bool bHit = PerformWeaponTraceComp->PerformStraightTraceFromCamera(Range, HitResultStraight, ECC_GameTraceChannel3);

	PerformPrimaryAction();
	
	if (bHit)
	{
		AActor* HitActor = HitResultStraight.GetActor();
		if (HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit actor with straight trace: %s"), *HitActor->GetName());

			if (HitActor->Implements<UDamageable>())
			{
				IDamageable* ActorToDamage{ Cast<IDamageable>(HitActor) };

				if (ActorToDamage)
				{
					ActorToDamage->DealDamage(WeaponDamage);
				}
			}
		}
	}
}

void ARW_SMG::PerformPrimaryAction()
{
	//UE_LOG(LogTemp, Warning, TEXT("Primary action regular from smg"));
}

void ARW_SMG::StartAutomaticFire()
{
	// Allows the player to hold down the input and have the weapon shoot automatically through the timer
	if (!GetWorld()->GetTimerManager().IsTimerActive(AutomaticFireTimerHandle))
	{
		PerformPrimaryActionAutomatic();

		bIsFiring = true;
		
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
	bIsWeaponActive = false;
}

void ARW_SMG::Reload(float InactivityDelay)
{
	Super::Reload(ReloadFlipbookLength + 0.5f);
}

void ARW_SMG::PlayShootFlipbook(float InactivityDelay)
{
	Super::PlayShootFlipbook(ShootFlipbookLength + 0.25f);
}

void ARW_SMG::PlayReloadFlipbook()
{
	Super::PlayReloadFlipbook();
}
