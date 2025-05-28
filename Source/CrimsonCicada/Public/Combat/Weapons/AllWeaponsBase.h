// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Pickupable.h"
#include "Interfaces/WeaponActions.h"
#include "Enums/EWeapon.h"
#include "Systems/Inventory/InventoryComponent.h"
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

	virtual void PickUp() override;

	UFUNCTION(BlueprintCallable)
	virtual void PerformPrimaryAction() override;

	UFUNCTION(BlueprintCallable)
	virtual void CastSpell() override;
	
	virtual void AddWeaponToInventory(EWeapon WeaponToAdd);

public:
	bool bCanBeEquipped{ true };

	UStaticMeshComponent* WeaponMesh;

	USkeletalMeshComponent* WeaponSkeletalMesh;

	UFUNCTION(BlueprintCallable)
	virtual EWeapon GetWeaponEnum() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 HandsRequired;
};
