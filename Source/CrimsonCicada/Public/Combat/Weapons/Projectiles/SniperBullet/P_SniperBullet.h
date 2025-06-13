// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P_SniperBullet.generated.h"

UCLASS()
class CRIMSONCICADA_API AP_SniperBullet : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AP_SniperBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComp;

	void LaunchProjectile();

	UFUNCTION(BlueprintCallable)
	void DestroyProjectile();

	class UCameraComponent* BulletCamera;

private:
	UStaticMeshComponent* BulletMesh;
	class UInventoryComponent* InventoryComp;
	APawn* PlayerPawn;
	
};
