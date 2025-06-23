// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONCICADA_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
public:
	ABaseAIController();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;


	//Sight Config
	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;
	//Hearing Config
	UPROPERTY()
	UAISenseConfig_Hearing* HearingConfig;
	//Handle Perception updated event
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	//Store current target
	UPROPERTY(BlueprintReadWrite, Category = "AI")
	AActor* CurrentTarget;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float MeleeRange = 200.0f;
	
};
