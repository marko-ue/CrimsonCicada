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
		if (!EquippedSpell || WeaponActor->HandsRequired != 2)
		{
			//WeaponActor->SetActorHiddenInGame(false);
			//WeaponActor->SetActorEnableCollision(true);
			if (WeaponActor->WeaponMesh)
			{
				WeaponActor->WeaponMesh->SetSimulatePhysics(false);
			}
			WeaponActor->EnableInput(GetWorld()->GetFirstPlayerController());

			EquippedWeapon = WeaponActor;
			WeaponActor->bCanBeEquipped = false;

			if (EquippedWeapon->IdleFlipbook)
			{
				WeaponFlipbookComp->SetFlipbook(EquippedWeapon->IdleFlipbook);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Equipped weapon does not have a flipbook selected."))
			}
			
		
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

void UInventoryComponent::UnequipSpell(EWeapon SpellToUnequip)
{
	ASpellsBase* SpellActor = Cast<ASpellsBase>(WeaponActorMap[SpellToUnequip]);

	if (WeaponActorMap.Contains(SpellToUnequip))
	{
		SpellActor = Cast<ASpellsBase>(WeaponActorMap[SpellToUnequip]);
	}
	
	if (SpellActor && EquippedSpell != nullptr)
	{
		SpellActor->SetActorHiddenInGame(true);
		SpellActor->SetActorEnableCollision(false);
		SpellActor->DisableInput(GetWorld()->GetFirstPlayerController());

		EquippedSpell = nullptr;
		SpellActor->bCanBeEquipped = true;
	}
}

