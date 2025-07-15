
#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/BaseEnemyCharacter.h"
#include "CricketEnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class ECricketState : uint8
{
	Inactive,
	Triggered,
	Territorial
};
UCLASS()
class CRIMSONCICADA_API ACricketEnemyCharacter : public ABaseEnemyCharacter
{
	GENERATED_BODY()
public:
	ACricketEnemyCharacter();
	virtual UBehaviorTree* GetBehaviorTree() const override;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cricket")
	UAnimMontage* LeapMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cricket")
	float LeapForce = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cricket")
	float TerritorialRadius = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cricket")
	float ExplosionRadius = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cricket")
	float ExplosionDamage = 20.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Cricket")
	ECricketState CurrentState = ECricketState::Inactive;
	UFUNCTION(BlueprintCallable, Category="Cricket")
	void StartLeap();
	UFUNCTION(BlueprintCallable, Category="Cricket")
	void LeapEnd();
	UFUNCTION(BlueprintCallable, Category="Cricket")
	void Explosion();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Cricket")
	bool bIsLeaping = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Cricket")
	bool bHasHitTarget = false;
	FVector RetreatLocation;
	bool bIsRetreating = false;
	
	FTimerHandle LeapTimerHandle;
	
	
};
