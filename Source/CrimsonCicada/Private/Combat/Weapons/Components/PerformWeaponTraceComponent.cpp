// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapons/Components/PerformWeaponTraceComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UPerformWeaponTraceComponent::UPerformWeaponTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPerformWeaponTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	CameraComp = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UCameraComponent>();
	
}


// Called every frame
void UPerformWeaponTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UPerformWeaponTraceComponent::PerformStraightTraceFromCamera(float WeaponRange, FHitResult& OutHit, ECollisionChannel TraceChannel)
{
	if (!CameraComp) { UE_LOG(LogTemp, Warning, TEXT("CameraComp is null")); return false; }
	
	FVector StartLocation{ CameraComp->GetComponentLocation() };
	FVector EndLocation{ StartLocation + CameraComp->GetForwardVector() * WeaponRange };
	
	FCollisionQueryParams IgnoreParams;
	IgnoreParams.AddIgnoredActor(GetOwner());
	IgnoreParams.AddIgnoredActor(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (bDebugMode)
	{
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, 2, 2);
	}

	return 
		GetWorld()->LineTraceSingleByChannel(
		OutHit,
		StartLocation,
		EndLocation,
		TraceChannel,
		IgnoreParams
	);
}

bool UPerformWeaponTraceComponent::PerformSpreadTraces(
	FVector TraceStart,
	FVector ForwardDirection,
	float SpreadAngleDegrees,
	int32 NumTraces,
	float TraceRange,
	TArray<FHitResult>& OutHits,
	ECollisionChannel TraceChannel)
{
	FCollisionQueryParams IgnoreParams;
	IgnoreParams.AddIgnoredActor(GetOwner());
	IgnoreParams.AddIgnoredActor(GetWorld()->GetFirstPlayerController()->GetPawn());

	bool bHitSomething = false;

	// For each number of traces specified, create a random direction in a cone with the specified spread degrees.
	// Perform a trace for each, then store the results of each in a hit result out parameter.
	// The out parameter is used to store the trace results in the hit result of the class that's calling.
	
	for (int32 i = 0; i < NumTraces; ++i)
	{
		FVector RandomDirection = FMath::VRandCone(
			ForwardDirection.GetSafeNormal(),
			FMath::DegreesToRadians(SpreadAngleDegrees)
		);

		FVector EndLocation = TraceStart + RandomDirection * TraceRange;

		FHitResult Hit;
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			Hit,
			TraceStart,
			EndLocation,
			TraceChannel,
			IgnoreParams
		);

		if (bHit)
		{
			OutHits.Add(Hit);
			bHitSomething = true;
		}

		if (bDebugMode)
		{
			DrawDebugLine(GetWorld(), TraceStart, EndLocation, FColor::Red, false, 1.0f, 0, 1.0f);
		}
	}

	return bHitSomething;
}

