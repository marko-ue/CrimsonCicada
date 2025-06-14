// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapons/RangedWeapons/RangedWeaponsBase.h"
#include "RW_SMG.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONCICADA_API ARW_SMG : public ARangedWeaponsBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	void BeginPlay() override;
	
	virtual EWeapon GetWeaponEnum() const override { return EWeapon::SMG; }
	
	virtual void PerformPrimaryActionAutomatic() override;

	virtual void StartAutomaticFire() override;
	virtual void StopAutomaticFire() override;

	FTimerHandle AutomaticFireTimerHandle;
	
	FTimerHandle SetWeaponActiveTimerHandle;

	virtual void Reload(float InactivityDelay) override;
	
private:
	float Range{ 2000.0f };

	virtual void PlayShootFlipbook(float InactivityDelay) override;
	virtual void PlayReloadFlipbook() override;

public:
	UPROPERTY(BlueprintReadWrite)
	bool bIsFiring;
};
