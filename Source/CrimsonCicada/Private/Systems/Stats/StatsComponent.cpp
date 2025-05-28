// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Stats/StatsComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatsComponent::ReduceHealth(float Amount)
{
	Stats[EStat::Health] -= Amount;
	Stats[EStat::Health] = UKismetMathLibrary::FClamp(
		Stats[EStat::Health], 0, Stats[EStat::MaxHealth]);

	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Stats[EStat::Health]);
}

float UStatsComponent::GetStat(EStat Stat) const
{
	if (const float* Value = Stats.Find(Stat))
	{
		return *Value;
	}
	
	return 0.f; // Or -1.f, or something meaningful if the stat doesn't exist
}

