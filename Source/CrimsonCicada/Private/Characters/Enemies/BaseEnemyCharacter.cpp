// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/BaseEnemyCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABaseEnemyCharacter::ABaseEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;            
	CurrentMovementSpeed = 0.0f;
}

// Called when the game starts or when spawned
void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseEnemyCharacter::TakeDamage(float DamageAmount)
{
	
}

float ABaseEnemyCharacter::GetMovementSpeed() const
{
	if (GetCharacterMovement())
	{
		return GetCharacterMovement()->Velocity.Size();
	}
	return 0;
}

void ABaseEnemyCharacter::PlayAttackMontage()
{
		if (AttackMontage)
		{
			PlayAnimMontage(AttackMontage);
		}
}



