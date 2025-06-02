// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/EStat.h"
#include "StatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRIMSONCICADA_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere)
	TMap<EStat, float> Stats;

	// Reduce function used with the AnyDamage actor event
	// Parameter from event used to reduce that actor's health
	UFUNCTION(BlueprintCallable)
	void ReduceHealth(float Amount);

	// Function for getting the specified stat value
	UFUNCTION(BlueprintCallable)
	float GetStat(EStat Stat) const;
};
