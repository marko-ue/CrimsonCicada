// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/RangedWeapons/DoubleDeagle/RW_DoubleDeagle.h"
#include "Kismet/KismetSystemLibrary.h"

void ARW_DoubleDeagle::BeginPlay()
{
	Super::BeginPlay();
	
	HandsRequired = 1;
	
	GetFlipbookLengthIfValid();
}

void ARW_DoubleDeagle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARW_DoubleDeagle::PerformPrimaryAction()
{
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
	if (!bIsWeaponActive)
	{
		WeaponFlipbookComp->SetFlipbook(ShootFlipbook);
		WeaponFlipbookComp->PlayFromStart();
		if (!bIsDualWieldSpellActive)
		{
			bIsWeaponActive = true;
		}
	}
	
	GetWorld()->GetTimerManager().SetTimer(SetWeaponInactiveTimerHandle, this, &AAllWeaponsBase::SetWeaponInactive, ShootFlipbookLength + 0.25f, false);

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
