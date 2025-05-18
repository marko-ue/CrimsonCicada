// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter/CicadaMainCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

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
	
}

// Called every frame
void ACicadaMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACicadaMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACicadaMainCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 1200;
}

void ACicadaMainCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = 600;
}

