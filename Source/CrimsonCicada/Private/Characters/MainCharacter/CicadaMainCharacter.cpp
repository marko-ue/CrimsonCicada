// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter/CicadaMainCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AkGameplayStatics.h"
#include "NavigationSystemTypes.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Combat/Weapons/AllWeaponsBase.h"
#include "Systems/Inventory/InventoryComponent.h"

// Sets default values
ACicadaMainCharacter::ACicadaMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACicadaMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnPlayerDeathDelegate.AddDynamic(this, &ACicadaMainCharacter::OnPlayerDeath);

	CameraComp = GetComponentByClass<UCameraComponent>();
	SpringArmComp = GetComponentByClass<USpringArmComponent>();
	MovementComp = GetCharacterMovement();
	InventoryComp = FindComponentByClass<UInventoryComponent>();
	WeaponFlipbookComp = Cast<UPaperFlipbookComponent>(GetDefaultSubobjectByName(TEXT("WeaponFlipbook")));
	SpellFlipbookComp = Cast<UPaperFlipbookComponent>(GetDefaultSubobjectByName(TEXT("SpellFlipbook")));
	WeaponDuelWieldFlipbookComp = Cast<UPaperFlipbookComponent>(GetDefaultSubobjectByName(TEXT("WeaponDuelWieldFlipbook")));
}

// Called every frame
void ACicadaMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Function that plays footsteps at the appopriate intervals based on player speed
	HandlePlayFootstepSounds();

	// Check if player is zooming
	if (bZoomingIn)
	{
		ZoomIn();
	}
	else
	{
		ZoomOut();
	}

}

// Called to bind functionality to input
void ACicadaMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACicadaMainCharacter::Sprint()
{
	MovementComp->MaxWalkSpeed = 1200;
}

void ACicadaMainCharacter::StopSprinting()
{
	MovementComp->MaxWalkSpeed = 600;
}

void ACicadaMainCharacter::Die()
{
	if (!bIsDead)
	{
		bIsDead = true;
		OnPlayerDeathDelegate.Broadcast();
	}
}

void ACicadaMainCharacter::OnPlayerDeath()
{
	MovementComp->MaxWalkSpeed = 0;

	AController* PlayerController = GetController();
	if (PlayerController)
	{
		PlayerController->SetIgnoreLookInput(true);
		PlayerController->SetIgnoreMoveInput(true);
		PlayerController->SetControlRotation(FRotator::ZeroRotator);
	}
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Settings for making the camera zoom out from the player's death location
	SpringArmComp->bUsePawnControlRotation = false;
	CameraComp->bUsePawnControlRotation = false;
	SpringArmComp->SetUsingAbsoluteRotation(true);
	CameraComp->SetRelativeRotation(FRotator::ZeroRotator);

	SpringArmComp->SetRelativeLocation(FVector(0.f, 0.f, 300.f));
	SpringArmComp->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	SpringArmComp->TargetArmLength = 600.f;

	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 2.f;

	
	FTimerHandle DeathWidgetTimerHandle;
	GetWorldTimerManager().SetTimer(
		DeathWidgetTimerHandle, this, &ACicadaMainCharacter::CreateDeathScreenWidget, 3.0f);
}

void ACicadaMainCharacter::CreateDeathScreenWidget()
{
	// Create the death widget and let the player use UI only
	DeathScreenWidget = CreateWidget<UUserWidget>(GetWorld(), DeathScreenWidgetClass);
	DeathScreenWidget->AddToViewport();
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	PC->SetInputMode(FInputModeUIOnly());
	PC->bShowMouseCursor = true;
}

void ACicadaMainCharacter::RestartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("RestartGame"));
	
	// Resets all settings previously impacted by player death
	APlayerController* PC = Cast<APlayerController>(GetController());
	PC->SetInputMode(FInputModeGameOnly());
	PC->bShowMouseCursor = false;

	// Removes death widget
	DeathScreenWidget->RemoveFromParent();
	DeathScreenWidget = nullptr;
	
	// Opens the level that was opened when the player died in a fresh state
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void ACicadaMainCharacter::SetAlternateMovementMode(bool bSetting)
{
	if (bIsInAlternateMovementMode == bSetting)
		return;

	bIsInAlternateMovementMode = bSetting;
	SetAlternateMovementModeSettings(bSetting);
}

void ACicadaMainCharacter::ZoomIn()
{
	bZoomingIn = true;
	
	float TargetFOV = 45;
	
	float NewFOV = UKismetMathLibrary::FInterpTo(CameraComp->FieldOfView, TargetFOV, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 10);
	CameraComp->FieldOfView = NewFOV;
}

void ACicadaMainCharacter::ZoomOut()
{
	bZoomingIn = false;
	
	float TargetFOV = 90;
	
	float NewFOV = UKismetMathLibrary::FInterpTo(CameraComp->FieldOfView, TargetFOV, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 10);
	CameraComp->FieldOfView = NewFOV;
}

void ACicadaMainCharacter::SetAlternateMovementModeSettings(bool State)
{
	if (State && MovementComp->GravityScale != 0.2f && SpringArmComp->bEnableCameraLag != true)
	{
		MovementComp->GravityScale = 0.2f;
		SpringArmComp->bEnableCameraLag = true;
		SpringArmComp->bEnableCameraRotationLag = true;
	}
	else if (!State && MovementComp->GravityScale != 1 && SpringArmComp->bEnableCameraLag != false)
	{
		MovementComp->GravityScale = 1;
		SpringArmComp->bEnableCameraLag = false;
		SpringArmComp->bEnableCameraRotationLag = false;
	}
}

void ACicadaMainCharacter::HandlePlayFootstepSounds()
{
	// If a footstep cannot be played (a footstep is already playing), return
	if (!bCanPlayFootstep)
	{
		return;
	}
	
	// If the player is falling, play the idle flipbook (instead of playing walk which is based off of velocity) and return
	if (MovementComp->IsFalling())
	{
		PlayIdleFlipbook();
		return;
	}
	
	// Always check if there's an equipped weapon and if that weapon is performing its action before playing idle
	if (MovementComp->Velocity.Size() <= 100 && InventoryComp->EquippedWeapon && !InventoryComp->EquippedWeapon->bIsWeaponActive)
	{
		PlayIdleFlipbook();
	}
	
	else if (MovementComp->Velocity.Size() >= 700)  // running
	{
		// Play footstep sound, then don't play another footstep until the timer is up
		UAkGameplayStatics::PostEvent(FootstepEvent, this, 0, FOnAkPostEventCallback());
		bCanPlayFootstep = false;
		
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this]() { bCanPlayFootstep = true; }, 0.25f, false);

		if (InventoryComp->EquippedWeapon && !InventoryComp->EquippedWeapon->bIsWeaponActive)
		{
			PlayRunFlipbook();
		}
	}
	else if (MovementComp->Velocity.Size() > 0)  // walking
	{
		// Play footstep sound, then don't play another footstep until the timer is up
		UAkGameplayStatics::PostEvent(FootstepEvent, this, 0, FOnAkPostEventCallback());
		bCanPlayFootstep = false;
		
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this]() { bCanPlayFootstep = true; }, 0.5f, false);

		if (InventoryComp->EquippedWeapon && !InventoryComp->EquippedWeapon->bIsWeaponActive)
		{
			PlayWalkFlipbook();
		}
	}
}

void ACicadaMainCharacter::PlayIdleFlipbook()
{ 
	// Return if the equipped weapon is performing its primary action (we don't want idle overriding it)
	if (InventoryComp->EquippedWeapon && InventoryComp->EquippedWeapon->bIsWeaponActive) return;

	// If the equipped weapon has a idle flipbook (nullptr check)
	if (InventoryComp->EquippedWeapon && InventoryComp->EquippedWeapon->IdleFlipbook)
	{
		// Play the idle flipbook from the start
		if (WeaponFlipbookComp->GetFlipbook() != InventoryComp->EquippedWeapon->IdleFlipbook)
		{
			WeaponFlipbookComp->SetFlipbook(InventoryComp->EquippedWeapon->IdleFlipbook);
			WeaponFlipbookComp->PlayFromStart();
		}
	}

	// If the equipped weapon has a idle flipbook (nullptr check)
	if (InventoryComp->EquippedWeapon && InventoryComp->EquippedWeapon->IdleFlipbook)
	{
		// If the dual wield spell is active, play the same flipbook but in the dual field flipbook component
		if (InventoryComp->EquippedWeapon->bIsDualWieldSpellActive)
		{
			if (WeaponDuelWieldFlipbookComp->GetFlipbook() != InventoryComp->EquippedWeapon->IdleFlipbook)
			{
				WeaponDuelWieldFlipbookComp->SetFlipbook(InventoryComp->EquippedWeapon->IdleFlipbook);
				WeaponDuelWieldFlipbookComp->PlayFromStart();
			}
		}
		else
		{
			// When the dual wield spell ends, remove the flipbook
			WeaponDuelWieldFlipbookComp->SetFlipbook(nullptr);
		}
	}
}

void ACicadaMainCharacter::PlayWalkFlipbook()
{
	// Return if the equipped weapon is performing its primary action (we don't want walk overriding it)
	if (InventoryComp->EquippedWeapon && InventoryComp->EquippedWeapon->bIsWeaponActive) return;
	
	// If the equipped weapon has the walk flipbook (nullptr check)
	if (InventoryComp->EquippedWeapon && InventoryComp->EquippedWeapon->WalkFlipbook)
	{
		// Play the walk flipbook from the start
		if (WeaponFlipbookComp->GetFlipbook() != InventoryComp->EquippedWeapon->WalkFlipbook)
		{
			WeaponFlipbookComp->SetFlipbook(InventoryComp->EquippedWeapon->WalkFlipbook);
			WeaponFlipbookComp->PlayFromStart();
		}
	}

	// If the equipped weapon has a walk flipbook (nullptr check)
	if (InventoryComp->EquippedWeapon && InventoryComp->EquippedWeapon->WalkFlipbook)
	{
		// If the dual wield spell is active, play the same flipbook but in the dual field flipbook component
		if (InventoryComp->EquippedWeapon->bIsDualWieldSpellActive)
		{
			if (WeaponDuelWieldFlipbookComp->GetFlipbook() != InventoryComp->EquippedWeapon->WalkFlipbook)
			{
				WeaponDuelWieldFlipbookComp->SetFlipbook(InventoryComp->EquippedWeapon->WalkFlipbook);
				WeaponDuelWieldFlipbookComp->PlayFromStart();
			}
		}
		else
		{
			// When the dual wield spell ends, remove the flipbook
			WeaponDuelWieldFlipbookComp->SetFlipbook(nullptr);
		}
	}
}

void ACicadaMainCharacter::PlayRunFlipbook()
{
	// Return if the equipped weapon is performing its primary action (we don't want walk overriding it)
	if (InventoryComp->EquippedWeapon && InventoryComp->EquippedWeapon->bIsWeaponActive) return;
	
	// If the equipped weapon has the run flipbook (nullptr check)
	if (InventoryComp->EquippedWeapon && InventoryComp->EquippedWeapon->RunFlipbook)
	{
		// Play the run flipbook from the start
		if (WeaponFlipbookComp->GetFlipbook() != InventoryComp->EquippedWeapon->RunFlipbook)
		{
			WeaponFlipbookComp->SetFlipbook(InventoryComp->EquippedWeapon->RunFlipbook);
			WeaponFlipbookComp->PlayFromStart();
		}
	}

	// If the equipped weapon has a run flipbook (nullptr check)
	if (InventoryComp->EquippedWeapon && InventoryComp->EquippedWeapon->RunFlipbook)
	{
		// If the dual wield spell is active, play the same flipbook but in the dual field flipbook component
		if (InventoryComp->EquippedWeapon->bIsDualWieldSpellActive)
		{
			if (WeaponDuelWieldFlipbookComp->GetFlipbook() != InventoryComp->EquippedWeapon->RunFlipbook)
			{
				WeaponDuelWieldFlipbookComp->SetFlipbook(InventoryComp->EquippedWeapon->RunFlipbook);
				WeaponDuelWieldFlipbookComp->PlayFromStart();
			}
		}
		else
		{
			// When the dual wield spell ends, remove the flipbook
			WeaponDuelWieldFlipbookComp->SetFlipbook(nullptr);
		}
	}
}


