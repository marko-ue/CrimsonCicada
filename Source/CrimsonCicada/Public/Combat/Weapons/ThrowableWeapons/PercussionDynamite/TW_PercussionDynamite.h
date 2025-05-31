// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapons/ThrowableWeapons/ThrowableWeaponsBase.h"
#include "TW_PercussionDynamite.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnExplosionSignature,
	ATW_PercussionDynamite, OnExplosionDelegate
);

UCLASS()
class CRIMSONCICADA_API ATW_PercussionDynamite : public AThrowableWeaponsBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	virtual EWeapon GetWeaponEnum() const override { return EWeapon::PercussionDynamite; }
	
	virtual void PerformPrimaryAction() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnExplosionSignature OnExplosionDelegate;
	
	UFUNCTION(BlueprintCallable)
	void Explode();

	UFUNCTION(BlueprintCallable)
	void OnExploded();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ImpactThreshold{ 200.0f };

	UPROPERTY(EditAnywhere)
	bool bDebugMode;
	
private:
	UFUNCTION()
	void SetShouldCheckForCollisions(bool ShouldCheckForCollisions);

	UPROPERTY(EditAnywhere)
	float ExplosionDamage{ 50 };

	UPROPERTY(EditAnywhere)
	float DamageRadius{ 500.0f };

};
