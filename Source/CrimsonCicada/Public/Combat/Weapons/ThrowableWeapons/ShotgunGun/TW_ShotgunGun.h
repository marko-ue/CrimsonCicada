// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapons/ThrowableWeapons/ThrowableWeaponsBase.h"
#include "TW_ShotgunGun.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONCICADA_API ATW_ShotgunGun : public AThrowableWeaponsBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	virtual EWeapon GetWeaponEnum() const override { return EWeapon::ShotgunGun; }

public:
	virtual void PerformPrimaryAction() override;

private:
	UFUNCTION()
	void SetShouldCheckForCollisions(bool ShouldCheckForCollisions);

	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintCallable)
	void OnFired();
	
	USceneComponent* BulletShootPoint;

	class UPerformWeaponTraceComponent* PerformWeaponTraceComp;

public:
	UPROPERTY(EditAnywhere, Category="Spread Trace")
	float SpreadAngleDegrees;
	UPROPERTY(EditAnywhere, Category="Spread Trace")
	float NumberOfTraces;
	UPROPERTY(EditAnywhere, Category="Spread Trace")
	float SpreadRange;
};

