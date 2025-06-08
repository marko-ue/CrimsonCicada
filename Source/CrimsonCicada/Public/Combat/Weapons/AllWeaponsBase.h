// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Pickupable.h"
#include "Interfaces/WeaponActions.h"
#include "Enums/EWeapon.h"
#include "Systems/Inventory/InventoryComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Camera/CameraComponent.h"
#include "AllWeaponsBase.generated.h"

UCLASS()
class CRIMSONCICADA_API AAllWeaponsBase : public AActor, public IPickupable, public IWeaponActions
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAllWeaponsBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	class UPerformWeaponTraceComponent* PerformWeaponTraceComp;
	
	UInventoryComponent* InventoryComp;

	// Handles picking up weapon for adding to the inventory
	virtual void PickUp() override;

	// Primary action for all weapons, override from interface
	UFUNCTION(BlueprintCallable)
	virtual void PerformPrimaryAction() override;

	// Cast spell for function used by spells, override from interface
	UFUNCTION(BlueprintCallable)
	virtual void CastSpell() override;
	
	virtual void AddWeaponToInventory(EWeapon WeaponToAdd);

public:
	bool bCanBeEquipped{ true };

	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* WeaponMesh;

	USkeletalMeshComponent* WeaponSkeletalMesh;

	UCameraComponent* CameraComp;

	// Used for getting the equipped weapon's enum. Useful for proper inheritance
	UFUNCTION(BlueprintCallable)
	virtual EWeapon GetWeaponEnum() const;

	// Variable that determines if a weapon is one handed or two handed
	// Weapons handle different based on the hands required
	// One handed weapons can be equipped at the same time as a spell, and dual wield works on them
	// Two handed weapons are genereally heavier and pack a bigger punch but lack other properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 HandsRequired;

	UPROPERTY(BlueprintReadOnly)
	bool bIsDualWieldSpellActive{ false };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsWeaponActive{ false };

	UFUNCTION()
	void SetWeaponInactive();

	FTimerHandle SetWeaponInactiveTimerHandle;
	
	// All flipbook components for the player each with their own purpose.
	UPaperFlipbookComponent* WeaponFlipbookComp;
	UPaperFlipbookComponent* SpellFlipbookComp;
	UPaperFlipbookComponent* WeaponDuelWieldFlipbookComp;

	// All weapons will have a flipbook that's played when idle, walking and running
	UPROPERTY(EditAnywhere)
	UPaperFlipbook* IdleFlipbook;
	UPROPERTY(EditAnywhere)
	UPaperFlipbook* WalkFlipbook;
	UPROPERTY(EditAnywhere)
	UPaperFlipbook* RunFlipbook;

	// In the all weapons base function, checks if the basic flipbooks are valid
	virtual void GetFlipbookLengthIfValid();

	UFUNCTION(BlueprintCallable)
	void ClearAllFlipbooks();
	//
};
