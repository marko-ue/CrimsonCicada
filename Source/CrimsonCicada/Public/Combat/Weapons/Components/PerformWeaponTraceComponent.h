// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PerformWeaponTraceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRIMSONCICADA_API UPerformWeaponTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPerformWeaponTraceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	class UCameraComponent* CameraComp;
	
	UPROPERTY(EditAnywhere)
	bool bDebugMode;

public:
	bool PerformStraightTraceFromCamera(float WeaponRange, FHitResult& OutHit, ECollisionChannel TraceChannel);
	bool PerformSpreadTraces(FVector TraceStart,
	FVector ForwardDirection,
	float SpreadAngleDegrees,
	int32 NumTraces,
	float TraceRange,
	TArray<FHitResult>& OutHits,
	ECollisionChannel TraceChannel);
};
