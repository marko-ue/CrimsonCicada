// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapons/AllWeaponsBase.h"
#include "Combat/Weapons/Components/PerformWeaponTraceComponent.h"
#include "RangedWeaponsBase.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONCICADA_API ARangedWeaponsBase : public AAllWeaponsBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARangedWeaponsBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintCallable)
	virtual void Reload(float InactivityDelay);

	FTimerHandle DelayWeaponInactiveTimerHandle;

	class UPerformWeaponTraceComponent* PerformWeaponTraceComp;
// Automatic weapons
	UPROPERTY(BlueprintReadOnly)
	bool bIsAutomatic;

	UPROPERTY(EditAnywhere)
	float FireRate;

	UFUNCTION(BlueprintCallable)
	virtual void StartAutomaticFire();

	UFUNCTION(BlueprintCallable)
	virtual void StopAutomaticFire();

	// Ranged weapon specific function for automatic weapons
	UFUNCTION()
	virtual void PerformPrimaryActionAutomatic();
//

	// Flipbook
	UPROPERTY(EditAnywhere)
	UPaperFlipbook* ShootFlipbook;
	float ShootFlipbookLength;
	
	UPROPERTY(EditAnywhere)
	UPaperFlipbook* ReloadFlipbook;
	float ReloadFlipbookLength;

	virtual void GetFlipbookLengthIfValid() override;
	// !!!!!!!!!!!! InactivityDelay is no longer in use. Remove when sure it won't ever be used again !!!!!!!!!! //
	virtual void PlayShootFlipbook(float InactivityDelay);
	virtual void PlayReloadFlipbook();

	// Ammo
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Ammo")
	int ClipSize;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Ammo")
	int AmmoInClip;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Ammo")
	int Ammo;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Ammo")
	int MaxAmmo;

	void ReduceAmmoInClipByAmount(float Amount) { AmmoInClip -= Amount; }

public:
	UPROPERTY(BlueprintReadOnly)
	bool bIsBulletStasisSpellActive{ false };

	UPROPERTY(EditAnywhere, Category="Sounds")
	UAkAudioEvent* WeaponShootSound;
	UPROPERTY(EditAnywhere, Category="Sounds")
	UAkAudioEvent* WeaponReloadSound;
	
};
