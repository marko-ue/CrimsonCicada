// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Inventory/InventoryComponent.h"
#include "Combat/Weapons/AllWeaponsBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddItemToInventory(EWeapon ItemID, int32 Amount)
{
	if (WeaponMap.Contains(ItemID))
	{
		WeaponMap[ItemID] += Amount;
	}
	else
	{
		WeaponMap.Add(ItemID, Amount);
	}
}

void UInventoryComponent::RemoveItemFromInventory(EWeapon ItemID, int32 Amount)
{
	if (WeaponMap.Contains(ItemID))
	{
		WeaponMap[ItemID] -= Amount;
	}
	else if (WeaponActorMap.Contains(ItemID))
	{
		WeaponActorMap[ItemID] -= Amount;
	}
}

void UInventoryComponent::StoreWeaponActor(EWeapon ItemID, class AAllWeaponsBase* WeaponActor)
{
	WeaponActorMap.Add(ItemID, WeaponActor);
}

void UInventoryComponent::EquipWeapon(EWeapon WeaponToEquip)
{
	AAllWeaponsBase* WeaponActor = nullptr;
	
	if (WeaponActorMap.Contains(WeaponToEquip))
	{
		WeaponActor = WeaponActorMap[WeaponToEquip];
	}
	
	if (WeaponActor && WeaponActor->bCanBeEquipped && EquippedWeapon == nullptr)
	{
		WeaponActor->SetActorHiddenInGame(false);
		WeaponActor->SetActorEnableCollision(true);
		if (WeaponActor->WeaponMesh)
		{
			WeaponActor->WeaponMesh->SetSimulatePhysics(false);
		}
		WeaponActor->EnableInput(GetWorld()->GetFirstPlayerController());

		EquippedWeapon = WeaponActor;
		WeaponActor->bCanBeEquipped = false;
		

		USkeletalMeshComponent* SkeletalMeshComp = Cast<USkeletalMeshComponent>(GetOwner()->GetDefaultSubobjectByName(TEXT("FirstPersonMesh")));
		if (SkeletalMeshComp && WeaponActor->WeaponMesh)
		{
			WeaponActor->WeaponMesh->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));
		}
		else if (SkeletalMeshComp && WeaponActor->WeaponSkeletalMesh)
		{
			WeaponActor->WeaponSkeletalMesh->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("A weapon is already equipped or selected weapon is not in inventory"));
	}
}

void UInventoryComponent::UnequipWeapon(EWeapon WeaponToUnequip)
{
	AAllWeaponsBase* WeaponActor = nullptr;
	
	if (WeaponActorMap.Contains(WeaponToUnequip))
	{
		WeaponActor = WeaponActorMap[WeaponToUnequip];
	}
	
	if (WeaponActor && EquippedWeapon != nullptr)
	{
		WeaponActor->SetActorHiddenInGame(true);
		WeaponActor->SetActorEnableCollision(false);
		WeaponActor->DisableInput(GetWorld()->GetFirstPlayerController());

		EquippedWeapon = nullptr;
		WeaponActor->bCanBeEquipped = true;
	}

	
}

