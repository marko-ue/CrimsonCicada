// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/BaseEnemyCharacter.h"
#include "Tombstone.generated.h"

UENUM(BlueprintType)
enum  class ETombstoneState : uint8
{
	Idling,
	Chasing,
	Attacking,
	
};
UCLASS()
class CRIMSONCICADA_API ATombstone : public ABaseEnemyCharacter
{
	GENERATED_BODY()
public:
	ATombstone();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual UBehaviorTree* GetBehaviorTree() const override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tombstone")
	UAnimMontage* SwingMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tombstone")
	float SwingRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Tombstone")
	ETombstoneState TombstoneState = ETombstoneState::Idling;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Tombstone")
	bool bIsAttacking = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Tombstone")
	bool bTargetTooClose = false;
	void StartAttack();
private:
	virtual void DealDamage(float DamageAmount) override;
};
