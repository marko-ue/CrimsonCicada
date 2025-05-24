// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapons/RangedWeapons/RangedWeaponsBase.h"
#include "RW_ShotgunGun.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONCICADA_API ARW_ShotgunGun : public ARangedWeaponsBase
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;
	
	virtual EWeapon GetWeaponEnum() const override { return EWeapon::ShotgunGun; }
	
	virtual void PerformPrimaryAction() override;

private:
	float Range{ 2000.0f };
	float SpreadRange{ 300.0f };

	USceneComponent* PelletShootSceneComponent;
};
