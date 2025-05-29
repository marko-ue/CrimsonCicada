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
}

// Called every frame
void ACicadaMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandlePlayFootstepSounds();

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
	// after a few seconds show widget for restarting and other options

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
	DeathScreenWidget = CreateWidget<UUserWidget>(GetWorld(), DeathScreenWidgetClass);
	DeathScreenWidget->AddToViewport();
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	PC->SetInputMode(FInputModeUIOnly());
	PC->bShowMouseCursor = true;
}

void ACicadaMainCharacter::RestartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("RestartGame"));
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	PC->SetInputMode(FInputModeGameOnly());
	PC->bShowMouseCursor = false;

	DeathScreenWidget->RemoveFromParent();
	DeathScreenWidget = nullptr;
	
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
	if (!bCanPlayFootstep || MovementComp->IsFalling())
		return;
	
	if (MovementComp->Velocity.Size() >= 700)  // running
	{
		UAkGameplayStatics::PostEvent(FootstepEvent, this, 0, FOnAkPostEventCallback());
		bCanPlayFootstep = false;
		
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this]() { bCanPlayFootstep = true; }, 0.25f, false);
	}
	else if (MovementComp->Velocity.Size() > 0)  // walking
	{
		UAkGameplayStatics::PostEvent(FootstepEvent, this, 0, FOnAkPostEventCallback());
		bCanPlayFootstep = false;
		
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this]() { bCanPlayFootstep = true; }, 0.5f, false);
	}
}



