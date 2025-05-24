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

void ARangedWeaponsBase::StartAutomaticFire()
{
}

void ARangedWeaponsBase::StopAutomaticFire()
{
}

void ARangedWeaponsBase::PerformPrimaryActionAutomatic()
{
}

