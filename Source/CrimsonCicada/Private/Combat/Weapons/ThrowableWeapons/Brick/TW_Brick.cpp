// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/ThrowableWeapons/Brick/TW_Brick.h"
#include "Camera/CameraComponent.h"


void ATW_Brick::BeginPlay()
{
	Super::BeginPlay();

	HandsRequired = 1;
	
	GetFlipbookLengthIfValid();
}

void ATW_Brick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

void ATW_Brick::PerformPrimaryAction()
{
	if (bIsWeaponActive) { return; }
	
	PlayThrowFlipbook();
}

void ATW_Brick::PlayThrowFlipbook()
{
	Super::PlayThrowFlipbook();
}
