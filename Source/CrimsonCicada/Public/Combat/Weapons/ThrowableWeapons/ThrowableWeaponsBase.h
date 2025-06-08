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
	
	UPROPERTY(BlueprintReadOnly)
	bool bShouldCheckForCollisions{ false };

	UPROPERTY(EditAnywhere, Category = "Throw Settings")
	FVector LaunchDirectionVector;
	UPROPERTY(EditAnywhere, Category = "Throw Settings")
	FVector LaunchForceVector;
	UPROPERTY(EditAnywhere, Category = "Throw Settings")
	FVector TorqueStrengthVector;

	// Flipbook
	UPROPERTY(EditAnywhere)
	UPaperFlipbook* ThrowFlipbook;
	float ThrowFlipbookLength;

	FTimerHandle RemoveFlipbookTimerHandle;

	FTimerHandle ApplyPhysicsTimerHandle;

public:
	void HandleWeaponThrown();

	UFUNCTION()
	void ClearThrowFlipbook();

	UFUNCTION()
	void ApplyThrowPhysics();

	virtual void PlayThrowFlipbook();
};
