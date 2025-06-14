// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/AllWeaponsBase.h"
#include "Camera/CameraComponent.h"
#include "Characters/MainCharacter/CicadaMainCharacter.h"
#include "GameFramework/Character.h"


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
	CameraComp = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UCameraComponent>();
	
	WeaponFlipbookComp = Cast<UPaperFlipbookComponent>(GetWorld()->GetFirstPlayerController()->GetPawn()->GetDefaultSubobjectByName(TEXT("WeaponFlipbook")));
	SpellFlipbookComp = Cast<UPaperFlipbookComponent>(GetWorld()->GetFirstPlayerController()->GetPawn()->GetDefaultSubobjectByName(TEXT("SpellFlipbook")));
	WeaponDuelWieldFlipbookComp = Cast<UPaperFlipbookComponent>(GetWorld()->GetFirstPlayerController()->GetPawn()->GetDefaultSubobjectByName(TEXT("WeaponDuelWieldFlipbook")));
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
	
	// Add the picked up weapon to inventory, with that equipped weapon's enum
	AddWeaponToInventory(GetWeaponEnum());
	// Stores the weapon actor for reference purposes
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

void AAllWeaponsBase::CastSpell()
{
	if (InventoryComp->EquippedWeapon != nullptr)
	{
		CastSpell();
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

void AAllWeaponsBase::SetWeaponInactive()
{
	bIsWeaponActive = false;

	ACharacter* Character = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	ACicadaMainCharacter* PlayerCharacter = Cast<ACicadaMainCharacter>(Character);
	PlayerCharacter->StartFlipbookCooldown();
	
	
}

void AAllWeaponsBase::GetFlipbookLengthIfValid()
{
	
}

void AAllWeaponsBase::ClearAllFlipbooks()
{
	WeaponFlipbookComp->Stop();
	WeaponFlipbookComp->SetFlipbook(nullptr);
	
	SpellFlipbookComp->Stop();
	SpellFlipbookComp->SetFlipbook(nullptr);

	WeaponDuelWieldFlipbookComp->Stop();
	WeaponDuelWieldFlipbookComp->SetFlipbook(nullptr);
}

