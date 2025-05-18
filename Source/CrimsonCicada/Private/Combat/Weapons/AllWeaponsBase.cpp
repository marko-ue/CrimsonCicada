// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/AllWeaponsBase.h"


// Sets default values
AAllWeaponsBase::AAllWeaponsBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAllWeaponsBase::BeginPlay()
{
	Super::BeginPlay();

	WeaponMesh = FindComponentByClass<UStaticMeshComponent>();
	WeaponSkeletalMesh = FindComponentByClass<USkeletalMeshComponent>();
	InventoryComp = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UInventoryComponent>();
}

// Called every frame
void AAllWeaponsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAllWeaponsBase::PickUp()
{
	if (WeaponMesh)
	{
		WeaponMesh->SetSimulatePhysics(false);
	}
	
	AddWeaponToInventory(GetWeaponEnum());
	InventoryComp->StoreWeaponActor(GetWeaponEnum(), this);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	DisableInput(GetWorld()->GetFirstPlayerController());
	bCanBeEquipped = true;

	UE_LOG(LogTemp, Warning, TEXT("Picked up"));
}

void AAllWeaponsBase::PerformPrimaryAction()
{
	if (InventoryComp->EquippedWeapon != nullptr)
	{
		PerformPrimaryAction();
	}
}

void AAllWeaponsBase::AddWeaponToInventory(EWeapon WeaponToAdd)
{
	InventoryComp->AddItemToInventory(WeaponToAdd, 1);
}

EWeapon AAllWeaponsBase::GetWeaponEnum() const
{
	return EWeapon::None;
}

