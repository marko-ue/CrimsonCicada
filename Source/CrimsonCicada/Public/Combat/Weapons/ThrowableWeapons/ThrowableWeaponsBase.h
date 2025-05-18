// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapons/AllWeaponsBase.h"
#include "ThrowableWeaponsBase.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONCICADA_API AThrowableWeaponsBase : public AAllWeaponsBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AThrowableWeaponsBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void AddWeaponToInventory(EWeapon WeaponToAdd) override;

	void HandleWeaponThrown();

	UPROPERTY(VisibleAnywhere, Category = "Throw Settings")
	FVector LaunchDirectionVector;
	UPROPERTY(VisibleAnywhere, Category = "Throw Settings")
	FVector LaunchForceVector;
	UPROPERTY(VisibleAnywhere, Category = "Throw Settings")
	FVector TorqueStrengthVector;
};
