// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/EWeapon.h"
#include "PaperFlipbookComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRIMSONCICADA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	TMap<EWeapon, int32> WeaponMap;
	TMap<EWeapon, class AAllWeaponsBase*> WeaponActorMap;
	TMap<EWeapon, class ASpellsBase*> SpellActorMap;

public:
	void AddItemToInventory(EWeapon ItemID, int32 Amount);
	void RemoveItemFromInventory(EWeapon ItemID, int32 Amount);
	void StoreWeaponActor(EWeapon ItemID, class AAllWeaponsBase* WeaponActor);
	
	UFUNCTION(BlueprintCallable)
	void EquipWeapon(EWeapon WeaponToEquip);
	UFUNCTION(BlueprintCallable)
	void EquipSpell (EWeapon SpellToEquip);
	
	UFUNCTION(BlueprintCallable)
	void UnequipWeapon(EWeapon WeaponToUnequip);
	UFUNCTION(BlueprintCallable)
	void UnequipSpell(EWeapon SpellToUnequip);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(BlueprintReadOnly)
	AAllWeaponsBase* EquippedWeapon;

	UPROPERTY(BlueprintReadOnly)
	class ASpellsBase* EquippedSpell;

private:
	// Flipbook
	UPaperFlipbookComponent* WeaponFlipbookComp;
	UPaperFlipbookComponent* SpellFlipbookComp;
};

