// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponActions.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponActions : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRIMSONCICADA_API IWeaponActions
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void PerformPrimaryAction() = 0;
	virtual void CastSpell() = 0;
};
