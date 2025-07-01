// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damageable.h"
#include "Stats/Stats.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BaseEnemyCharacter.generated.h"

UCLASS()
class CRIMSONCICADA_API ABaseEnemyCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemyCharacter();
	virtual UBehaviorTree* GetBehaviorTree() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void TakeDamage(float DamageAmount) override;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	float GetMovementSpeed() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AttackMontage;

	UFUNCTION(BlueprintNativeEvent, Category = "Combat")
	void PerformAttack();
	virtual void PerformAttack_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Movement")
	void PerformSpecialMove();
	virtual void PerformSpecialMove_Implementation();
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float CurrentMovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* CustomBehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = 0.0))
	float Health = 100.0f;
	
	
};
