// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/EnemyBase.h"
#include "Interfaces/Damageable.h"
#include "Lich.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONCICADA_API ALich : public AEnemyBase, public IDamageable
{
	GENERATED_BODY()
	
};
