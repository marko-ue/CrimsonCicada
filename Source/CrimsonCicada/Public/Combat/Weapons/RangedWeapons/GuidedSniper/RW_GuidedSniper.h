// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapons/RangedWeapons/RangedWeaponsBase.h"
#include "Combat/Weapons/Projectiles/SniperBullet/P_SniperBullet.h"
#include "RW_GuidedSniper.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONCICADA_API ARW_GuidedSniper : public ARangedWeaponsBase
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual EWeapon GetWeaponEnum() const override { return EWeapon::GuidedSniper; }

	virtual void PerformPrimaryAction() override;

private:
	float Range{ 2000.0f };

	UPROPERTY(EditAnywhere)
	bool bDebugMode;

	virtual void PlayShootFlipbook(float InactivityDelay) override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AP_SniperBullet> ProjectileClass;
};
