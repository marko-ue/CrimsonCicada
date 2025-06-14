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

	if (AmmoInClip <= 0) { return; }
	
	PlayShootFlipbook(ShootFlipbookLength + 0.25f);
	
	//* Straight trace *\\
	
	FHitResult HitResultStraight;
	bool bHit = PerformWeaponTraceComp->PerformStraightTraceFromCamera(Range, HitResultStraight, ECC_GameTraceChannel3);
	ReduceAmmoInClipByAmount(1);

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

	ReduceAmmoInClipByAmount(1);

	if (bActorHit)
	{
		AActor* HitActor = HitResultRandom.GetActor();
		if (HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit actor with random trace: %s"), *HitActor->GetName());
		}
	}
}

void ARW_DoubleDeagle::Reload(float InactivityDelay)
{
	Super::Reload(ReloadFlipbookLength - 0.2f);
}

void ARW_DoubleDeagle::PlayShootFlipbook(float InactivityDelay)
{
	Super::PlayShootFlipbook(ShootFlipbookLength + 0.25f);
}

void ARW_DoubleDeagle::PlayReloadFlipbook()
{
	Super::PlayReloadFlipbook();
}
