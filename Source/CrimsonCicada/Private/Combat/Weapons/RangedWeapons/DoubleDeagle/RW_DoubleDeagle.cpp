// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/RangedWeapons/DoubleDeagle/RW_DoubleDeagle.h"
#include "Kismet/KismetSystemLibrary.h"

void ARW_DoubleDeagle::BeginPlay()
{
	Super::BeginPlay();
	
	HandsRequired = 1;
	
	// Function that tries to get basic flipbooks from weapons, so you don't have to check every time
	GetFlipbookLengthIfValid();
}

void ARW_DoubleDeagle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARW_DoubleDeagle::PerformPrimaryAction()
{
	// This weapon shouldn't shoot if it's already shooting. Return the function if it's already doing so.
	if (bIsWeaponActive) { return; }
	
	PlayShootFlipbook();
	
	//* Straight trace *\\
	
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
	
	//* Random trace *\\
	
	// Like a regular trace, but multiplied with a random location on all axes
	FVector StartLocation{ CameraComp->GetComponentLocation() };
	FVector RandomDirection = FVector(FMath::RandRange(5, 50), FMath::RandRange(5, 50), FMath::RandRange(5, 50));
	FVector EndLocation{ StartLocation + CameraComp->GetForwardVector() * RandomDirection * Range };

	FHitResult HitResultRandom;
	FCollisionQueryParams IgnoreParams;
	IgnoreParams.AddIgnoredActor(this);

	if (bDebugMode)
	{
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, 2, 2);
	}
	
	bool bActorHit{
		GetWorld()->LineTraceSingleByChannel(
		HitResultRandom,
		StartLocation,
		EndLocation,
		ECC_GameTraceChannel2,
		IgnoreParams
	) };

	if (bActorHit)
	{
		AActor* HitActor = HitResultRandom.GetActor();
		if (HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit actor with random trace: %s"), *HitActor->GetName());
		}
	}
}

void ARW_DoubleDeagle::PlayShootFlipbook()
{
	// Plays the shoot flipbook if not already playing/weapon active
	if (!bIsWeaponActive)
	{
		WeaponFlipbookComp->SetFlipbook(ShootFlipbook);
		WeaponFlipbookComp->PlayFromStart();
		// Check if dual wield spell is active
		if (!bIsDualWieldSpellActive)
		{
			bIsWeaponActive = true;
		}
	}
	
	// Makes the weapon inactive after the duration of the flipbook and some extra to avoid bugs
	GetWorld()->GetTimerManager().SetTimer(SetWeaponInactiveTimerHandle, this, &AAllWeaponsBase::SetWeaponInactive, ShootFlipbookLength + 0.25f, false);

	// If the dual wield spell is active, do the same but for the other weapon
	if (bIsDualWieldSpellActive)
	{
		if (!bIsWeaponActive)
		{
			WeaponDuelWieldFlipbookComp->SetFlipbook(ShootFlipbook);
			WeaponDuelWieldFlipbookComp->PlayFromStart();
			bIsWeaponActive = true;
		}
		GetWorld()->GetTimerManager().SetTimer(SetWeaponInactiveTimerHandle, this, &AAllWeaponsBase::SetWeaponInactive, ShootFlipbookLength + 0.25f, false);
	}
}
