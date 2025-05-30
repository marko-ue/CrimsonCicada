// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AkAudioEvent.h"
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

	UFUNCTION(BlueprintCallable)
	void SetAlternateMovementMode(bool bSetting);
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsInAlternateMovementMode{ false };

	UPROPERTY(EditAnywhere, Category="Sounds")
	UAkAudioEvent* FootstepEvent;

	UFUNCTION(BlueprintCallable)
	void ZoomIn();
	UFUNCTION(BlueprintCallable)
	void ZoomOut();

	bool bZoomingIn{ false };

	
	
private:
	class UCameraComponent* CameraComp;
	UCharacterMovementComponent* MovementComp;
	class USpringArmComponent* SpringArmComp;
	class UInventoryComponent* InventoryComp;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> DeathScreenWidgetClass;
	UUserWidget* DeathScreenWidget;

	void SetAlternateMovementModeSettings(bool State);
	
	bool bIsDead{ false };

	void HandlePlayFootstepSounds();
	
	bool bCanPlayFootstep{ true };

	UFUNCTION(BlueprintCallable)
	void PlayWalkFlipbook();
	UFUNCTION(BlueprintCallable)
	void PlayIdleFlipbook();

	// Flipbook
	class UPaperFlipbookComponent* WeaponFlipbookComp;
	class UPaperFlipbookComponent* SpellFlipbookComp;
	class UPaperFlipbookComponent* WeaponDuelWieldFlipbookComp;
	
};
