// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/Projectiles/SniperBullet/P_SniperBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Combat/Weapons/AllWeaponsBase.h"
#include "Systems/Inventory/InventoryComponent.h"

// Sets default values
AP_SniperBullet::AP_SniperBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Projectile movement comp default settings
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 1500.f;
	ProjectileMovementComp->MaxSpeed = 5000.f;
	ProjectileMovementComp->bShouldBounce = false;
	ProjectileMovementComp->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AP_SniperBullet::BeginPlay()
{
	Super::BeginPlay();

	BulletMesh = FindComponentByClass<UStaticMeshComponent>();
	BulletCamera = FindComponentByClass<UCameraComponent>();
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	InventoryComp = PlayerPawn->FindComponentByClass<UInventoryComponent>();
}

// Called every frame
void AP_SniperBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AP_SniperBullet::LaunchProjectile()
{
	BulletMesh->IgnoreActorWhenMoving(PlayerPawn, true);
	
	ProjectileMovementComp->SetVelocityInLocalSpace(FVector::ForwardVector * ProjectileMovementComp->InitialSpeed);
	ProjectileMovementComp->Activate();
}

void AP_SniperBullet::DestroyProjectile()
{
	// Set camera back to the player
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(PlayerPawn, 0.3f);
	InventoryComp->EquippedWeapon->bIsWeaponActive = false;
	Destroy();
}

