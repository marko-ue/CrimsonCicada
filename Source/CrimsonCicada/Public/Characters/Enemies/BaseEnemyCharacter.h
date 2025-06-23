// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damageable.h"
#include "Stats/Stats.h"
#include "BaseEnemyCharacter.generated.h"

UCLASS()
class CRIMSONCICADA_API ABaseEnemyCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Taking damage
	virtual void TakeDamage(float DamageAmount) override;
	UFUNCTION(BlueprintCallable, Category = "Animation")
	float GetMovementSpeed() const;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AttackMontage;
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void PlayAttackMontage();
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float CurrentMovementSpeed;
	
	
};
