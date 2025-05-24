// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapons/RangedWeapons/RangedWeaponsBase.h"
#include "Combat/Weapons/ThrowableWeapons/ShotgunGun/TW_ShotgunGun.h"
#include "RW_Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONCICADA_API ARW_Shotgun : public ARangedWeaponsBase
{
	GENERATED_BODY()
	
protected:
	void BeginPlay() override;
	
	virtual EWeapon GetWeaponEnum() const override { return EWeapon::ShotgunGun; }

	virtual void PerformPrimaryAction() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<ATW_ShotgunGun> ProjectileClass;

private:
	USceneComponent* BulletShootPoint;
	
};
