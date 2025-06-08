// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/Projectiles/RicochetBullet/P_RicochetBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AP_RicochetBullet::AP_RicochetBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Projectile movement comp default settings
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 1500.f;
	ProjectileMovementComp->MaxSpeed = 5000.f;
	ProjectileMovementComp->bShouldBounce = true;
	ProjectileMovementComp->Bounciness = 0.8f;
	ProjectileMovementComp->ProjectileGravityScale = 0.f;

	
}

// Called when the game starts or when spawned
void AP_RicochetBullet::BeginPlay()
{
	Super::BeginPlay();

	BulletMesh = FindComponentByClass<UStaticMeshComponent>();
}

// Called every frame
void AP_RicochetBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AP_RicochetBullet::LaunchProjectile()
{
	BulletMesh->IgnoreActorWhenMoving(GetWorld()->GetFirstPlayerController()->GetPawn(), true);
	
	ProjectileMovementComp->SetVelocityInLocalSpace(FVector::ForwardVector * ProjectileMovementComp->InitialSpeed);
	ProjectileMovementComp->Activate();
}

void AP_RicochetBullet::DestroyProjectile()
{
	Destroy();
}



