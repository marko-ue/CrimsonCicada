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

bool UPerformWeaponTraceComponent::PerformSpreadTraces(FVector TraceStart, float SpreadRange, FHitResult& OutHit, ECollisionChannel TraceChannel)
{
	FVector StartLocation{ TraceStart };
	FVector EndLocation{ StartLocation + StartLocation.Z * SpreadRange };

	FCollisionQueryParams IgnoreParams;
	IgnoreParams.AddIgnoredActor(GetOwner());
	IgnoreParams.AddIgnoredActor(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (bDebugMode)
	{
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, 2, 2);
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

