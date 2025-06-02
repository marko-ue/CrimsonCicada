// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Interaction/InteractComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Interfaces/Pickupable.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	CameraComp = GetOwner()->FindComponentByClass<UCameraComponent>();
	
}


// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractComponent::PerformInteractTrace()
{
	FVector StartLocation{ CameraComp->GetComponentLocation() };
	FVector EndLocation{ StartLocation + CameraComp->GetForwardVector() * PickupRange };

	FHitResult InteractableHit;
	FCollisionQueryParams IgnoreParams;
	IgnoreParams.AddIgnoredActor(GetOwner());

	if (bDebugMode)
	{
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, 2, 2);
	}
	
	bool bInteractableHit{
		GetWorld()->LineTraceSingleByChannel(
		InteractableHit,
		StartLocation,
		EndLocation,
		ECC_GameTraceChannel2,
		IgnoreParams
	) };

	if (bInteractableHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractableHit"));
		AActor* HitActor{ InteractableHit.GetActor() };
		
		// If there's an actor hit, checks if it implements the pickupable interface
		if (HitActor && HitActor->Implements<UPickupable>())
		{
			// Cast to pickupable
			IPickupable* ItemToPickUp{ Cast<IPickupable>(HitActor) };

			if (ItemToPickUp)
			{
				// Call the PickUp function on the hit actor
				ItemToPickUp->PickUp();
			}
		}
	}
}

