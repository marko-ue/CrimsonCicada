// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P_RicochetBullet.generated.h"

UCLASS()
class CRIMSONCICADA_API AP_RicochetBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AP_RicochetBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class UProjectileMovementComponent* ProjectileMovementComp;

	void LaunchProjectile();
};
