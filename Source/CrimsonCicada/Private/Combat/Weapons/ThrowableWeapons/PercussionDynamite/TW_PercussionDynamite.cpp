// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/ThrowableWeapons/PercussionDynamite/TW_PercussionDynamite.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void ATW_PercussionDynamite::BeginPlay()
{
	Super::BeginPlay();
	
	HandsRequired = 1;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
}

void ATW_PercussionDynamite::PerformPrimaryAction()
{
	// Launches the weapon in the direction of the camera's forward vector.
	// Makes the weapon visible and enable physics.

	LaunchDirectionVector = CameraComp->GetForwardVector();
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->AddImpulse(LaunchDirectionVector * LaunchForceVector);
	WeaponMesh->AddTorqueInRadians(TorqueStrengthVector, NAME_None, true);

	// Function for handling thrown weapon (setting equipped weapon to none, detaching and making the weapon equippable)
	HandleWeaponThrown();

	// Timer that will allow collisions to be checked by making a bool true, bool checked in blueprint
	FTimerHandle SetCollisionsHandle;
	FTimerDelegate SetCollisionsDelegate = FTimerDelegate::CreateUObject(this, &ATW_PercussionDynamite::SetShouldCheckForCollisions, true);
	GetWorldTimerManager().SetTimer(SetCollisionsHandle, SetCollisionsDelegate, 0.4, false);
}

void ATW_PercussionDynamite::SetShouldCheckForCollisions(bool ShouldCheckForCollisions)
{
	bShouldCheckForCollisions = ShouldCheckForCollisions;
}


void ATW_PercussionDynamite::Explode()
{
	UE_LOG(LogTemp, Warning, TEXT("Exploded"));
	TArray<AActor*> IgnoredActors;
	
	UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, WeaponMesh->GetComponentLocation(), DamageRadius,
		UDamageType::StaticClass(), IgnoredActors);

	if (bDebugMode)
	{
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), WeaponMesh->GetComponentLocation(), DamageRadius, 32, FColor::Red, 2, 2);
	}

	// Broadcasts delegate which will be used to notify enemies so they run away and other behavior
	OnExplosionDelegate.Broadcast();
}

void ATW_PercussionDynamite::OnExploded()
{
	Destroy();
}
