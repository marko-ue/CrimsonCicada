 

#include "BTTasks/LeapTaskNode.h"
#include "Characters/Enemies/RubyCricket/CricketEnemyCharacter.h"
#include "AI/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"


ULeapTaskNode::ULeapTaskNode()
{
  NodeName = "LeapTowardsTarget";
}
EBTNodeResult::Type ULeapTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;
	ACricketEnemyCharacter* Cricket = Cast<ACricketEnemyCharacter>(Controller->GetPawn());
	if (Cricket && !Cricket->bIsLeaping)
	{
		Cricket->StartLeap();
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsLeaping"), true);
		return EBTNodeResult::InProgress;
		
	}
	return EBTNodeResult::Failed;
}


