
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapons/AllWeaponsBase.h"
#include "SpellsBase.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONCICADA_API ASpellsBase : public AAllWeaponsBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASpellsBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Time until spell can be activated again, duration of the spell once activated, and check if the spell is on cooldown
	float SpellCooldown;
	float SpellDuration;
	bool bIsSpellOnCooldown{ false };
};
