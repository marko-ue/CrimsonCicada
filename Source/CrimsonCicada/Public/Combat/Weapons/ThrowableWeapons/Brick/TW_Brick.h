// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapons/ThrowableWeapons/ThrowableWeaponsBase.h"
#include "TW_Brick.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONCICADA_API ATW_Brick : public AThrowableWeaponsBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	virtual EWeapon GetWeaponEnum() const override { return EWeapon::Brick; }
	
	virtual void PerformPrimaryAction() override;
};
