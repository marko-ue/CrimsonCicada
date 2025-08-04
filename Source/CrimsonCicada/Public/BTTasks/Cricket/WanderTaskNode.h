#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "WanderTaskNode.generated.h"

UCLASS()
class CRIMSONCICADA_API UWanderTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UWanderTaskNode();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Wander")
	float WanderRadius = 1000.f;
};
