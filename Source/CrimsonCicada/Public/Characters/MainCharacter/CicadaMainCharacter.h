// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CicadaMainCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnPlayerDeathSignature,
	ACicadaMainCharacter, OnPlayerDeathDelegate
);

UCLASS()
class CRIMSONCICADA_API ACicadaMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACicadaMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDeathSignature OnPlayerDeathDelegate;

	UFUNCTION(BlueprintCallable)
	void Sprint();
	UFUNCTION(BlueprintCallable)
	void StopSprinting();

	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION(BlueprintCallable)
	void RestartGame();

	UFUNCTION()
	void OnPlayerDeath();

	UFUNCTION()
	void CreateDeathScreenWidget();

private:
	class UCameraComponent* CameraComp;
	UCharacterMovementComponent* MovementComp;
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> DeathScreenWidgetClass;
	UUserWidget* DeathScreenWidget;
	
	bool bIsDead{ false };
};
