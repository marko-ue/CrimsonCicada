// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapons/RangedWeapons/RangedWeaponsBase.h"
#include "RW_DoubleDeagle.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONCICADA_API ARW_DoubleDeagle : public ARangedWeaponsBase
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual EWeapon GetWeaponEnum() const override { return EWeapon::DoubleDeagle; }

	virtual void PerformPrimaryAction() override;
	
	virtual void Reload(float InactivityDelay) override;

private:
	float Range{ 2000.0f };

	UPROPERTY(EditAnywhere)
	bool bDebugMode;

	virtual void PlayShootFlipbook(float InactivityDelay) override;
	virtual void PlayReloadFlipbook() override;
};
