// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapons/Spells/SpellsBase.h"
#include "S_TimeStop.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONCICADA_API AS_TimeStop : public ASpellsBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
	virtual EWeapon GetWeaponEnum() const override { return EWeapon::TimeStop; }
	
public:
	virtual void CastSpell() override;

private:
	
};
