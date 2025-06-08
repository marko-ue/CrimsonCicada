// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapons/RangedWeapons/RangedWeaponsBase.h"
#include "Combat/Weapons/Projectiles/RicochetBullet/P_RicochetBullet.h"
#include "RW_RicochetRevolver.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONCICADA_API ARW_RicochetRevolver : public ARangedWeaponsBase
{
	GENERATED_BODY()
	
protected:
	void BeginPlay() override;
	
	virtual EWeapon GetWeaponEnum() const override { return EWeapon::RicochetRevolver; }

	virtual void PerformPrimaryAction() override;

private:
	float Range{ 2000.0f };

	USceneComponent* BulletShootPoint;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AP_RicochetBullet> ProjectileClass;
};
