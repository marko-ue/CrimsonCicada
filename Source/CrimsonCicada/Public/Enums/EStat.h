// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Enums used for the stats component (for the player, enemies, and meshes)
UENUM(BlueprintType)
enum class EStat : uint8
{
	Health,
	MaxHealth UMETA(DisplayName = "Max Health"),
};

