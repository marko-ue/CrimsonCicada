// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Inventory/InventoryComponent.h"
#include "Combat/Weapons/AllWeaponsBase.h"
#include "Combat/Weapons/Spells/SpellsBase.h"

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

	WeaponFlipbookComp = Cast<UPaperFlipbookComponent>(GetWorld()->GetFirstPlayerController()->GetPawn()->GetDefaultSubobjectByName(TEXT("WeaponFlipbook")));
	SpellFlipbookComp = Cast<UPaperFlipbookComponent>(GetWorld()->GetFirstPlayerController()->GetPawn()->GetDefaultSubobjectByName(TEXT("SpellFlipbook")));
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddItemToInventory(EWeapon ItemID, int32 Amount)
{
	// If the item is already in the inventory, just add the extra amount, otherwise create a new pair
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
	// Removes the weapon and weapon actor from the map
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
	// Adds the specified weapon's actor to the map
	WeaponActorMap.Add(ItemID, WeaponActor);
}

void UInventoryComponent::EquipWeapon(EWeapon WeaponToEquip)
{
	// First sets the current weapon actor to none
	AAllWeaponsBase* WeaponActor = nullptr;
	
	// Sets the variable to be the specified weapon's actor if it exists in the inventory
	if (WeaponActorMap.Contains(WeaponToEquip))
	{
		WeaponActor = WeaponActorMap[WeaponToEquip];
	}
	
	if (WeaponActor && WeaponActor->bCanBeEquipped && EquippedWeapon == nullptr)
	{
		// Only equip the weapon if the weapon is not 2 handed
		if (!EquippedSpell || WeaponActor->HandsRequired != 2)
		{
			if (WeaponActor->WeaponMesh)
			{
				WeaponActor->WeaponMesh->SetSimulatePhysics(false);
			}
			WeaponActor->EnableInput(GetWorld()->GetFirstPlayerController());

			// Variable used globally to call functions on the equipped weapon
			EquippedWeapon = WeaponActor;
			WeaponActor->bCanBeEquipped = false;

			// Play that weapon's idle flipbook initially
			if (EquippedWeapon->IdleFlipbook)
			{
				WeaponFlipbookComp->SetFlipbook(EquippedWeapon->IdleFlipbook);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Equipped weapon does not have a flipbook selected. Equipping weapon without flipbook."))
			}

			// Set up attachment to skeletal mesh. Legacy code
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
			UE_LOG(LogTemp, Error, TEXT("Cannot equip a two-handed weapon when a one-handed spell is equipped or a two-handed spell is equipped"));
		}
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("A weapon is already equipped or selected weapon is not in inventory"));
	}
}

void UInventoryComponent::EquipSpell(EWeapon SpellToEquip)
{
	
	if (!WeaponActorMap.Contains(SpellToEquip))
	{
		UE_LOG(LogTemp, Error, TEXT("Spell not in inventory"));
		return;
	}

	if (EquippedWeapon && EquippedWeapon->HandsRequired != 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot equip spell. Equipped weapon is two-handed."));
		return;
	}

	if (EquippedSpell != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spell already equipped."));
		return;
	}

	ASpellsBase* SpellActor = Cast<ASpellsBase>(WeaponActorMap[SpellToEquip]);
	if (!SpellActor)
	{
		UE_LOG(LogTemp, Error, TEXT("SpellActor not valid."));
		return;
	}

	SpellActor->EnableInput(GetWorld()->GetFirstPlayerController());
	SpellActor->SetActorHiddenInGame(false);
	SpellActor->SetActorEnableCollision(true);
	
	if (SpellActor->WeaponMesh)
	{
		SpellActor->WeaponMesh->SetSimulatePhysics(false);
	}
	
	SpellActor->EnableInput(GetWorld()->GetFirstPlayerController());

	// Set up attachment to skeletal mesh. Legacy code
	USkeletalMeshComponent* SkeletalMeshComp = Cast<USkeletalMeshComponent>(GetOwner()->GetDefaultSubobjectByName(TEXT("FirstPersonMesh")));
	if (SkeletalMeshComp && SpellActor->WeaponMesh)
	{
		SpellActor->WeaponMesh->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));
	}
	else if (SkeletalMeshComp && SpellActor->WeaponSkeletalMesh)
	{
		SpellActor->WeaponSkeletalMesh->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));
	}
	
	EquippedSpell = SpellActor;
}

void UInventoryComponent::UnequipWeapon(EWeapon WeaponToUnequip)
{
	// Sets the weapon actor to none first
	AAllWeaponsBase* WeaponActor = nullptr;
	
	// Unequip only if there's an actor map
	if (WeaponActorMap.Contains(WeaponToUnequip))
	{
		WeaponActor = WeaponActorMap[WeaponToUnequip];
	}
	
	// Set settings for unequipped weapon (make invisible and set equipped weapon to nullptr)
	if (WeaponActor && EquippedWeapon != nullptr)
	{
		WeaponActor->SetActorHiddenInGame(true);
		WeaponActor->SetActorEnableCollision(false);
		WeaponActor->DisableInput(GetWorld()->GetFirstPlayerController());

		EquippedWeapon = nullptr;
		WeaponActor->bCanBeEquipped = true;
	}
}

void UInventoryComponent::UnequipSpell(EWeapon SpellToUnequip)
{
	// Sets the spell actor to none
	ASpellsBase* SpellActor = Cast<ASpellsBase>(WeaponActorMap[SpellToUnequip]);

	// Unequip if the actor map contains the spell
	if (WeaponActorMap.Contains(SpellToUnequip))
	{
		SpellActor = Cast<ASpellsBase>(WeaponActorMap[SpellToUnequip]);
	}

	// Set settings for unequipped weapon (make invisible and set equipped weapon to nullptr)
	if (SpellActor && EquippedSpell != nullptr)
	{
		SpellActor->SetActorHiddenInGame(true);
		SpellActor->SetActorEnableCollision(false);
		SpellActor->DisableInput(GetWorld()->GetFirstPlayerController());

		EquippedSpell = nullptr;
		SpellActor->bCanBeEquipped = true;
	}
}

