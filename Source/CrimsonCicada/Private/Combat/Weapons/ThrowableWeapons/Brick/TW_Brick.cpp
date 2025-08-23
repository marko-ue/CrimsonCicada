// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/ThrowableWeapons/Brick/TW_Brick.h"
#include "Camera/CameraComponent.h"


void ATW_Brick::BeginPlay()
{
	Super::BeginPlay();

	HandsRequired = 1;
	
	GetFlipbookLengthIfValid();

	WeaponFlipbookComp->OnFinishedPlaying.AddDynamic(this, &AAllWeaponsBase::SetWeaponInactive);
}

void ATW_Brick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//UE_LOG(LogTemp, Warning, TEXT("Custom time dilation: %f"), CustomTimeDilation);
}

void ATW_Brick::PerformPrimaryAction()
{
	if (bIsWeaponActive) { return; }
	
	PlayThrowFlipbook(ThrowFlipbookLength + 0.33f);

	FTimerHandle SetCollisionsHandle;
	FTimerDelegate SetCollisionsDelegate = FTimerDelegate::CreateUObject(this, &ATW_Brick::SetShouldCheckForCollisions, true);
	GetWorldTimerManager().SetTimer(SetCollisionsHandle, SetCollisionsDelegate, 0.4, false);
}

void ATW_Brick::PlayThrowFlipbook(float RemoveFlipbookDelay)
{
	Super::PlayThrowFlipbook(ThrowFlipbookLength + 0.33f);
}

void ATW_Brick::SetShouldCheckForCollisions(bool ShouldCheckForCollisions)
{
	bShouldCheckForCollisions = ShouldCheckForCollisions;
}
