// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// All weapon enums (melee, ranged, throwable, spells) used as identification for each weapon
UENUM(BlueprintType)
enum class EWeapon : uint8
{
	None,
	Brick,
	DoubleDeagle,
	SMG,
	ShotgunGun,
	TimeStop,
	DualWield,
	PercussionDynamite,
	RicochetRevolver
};
