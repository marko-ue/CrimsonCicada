#include "BTTasks/WanderTaskNode.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"

UWanderTaskNode::UWanderTaskNode()
{
	NodeName = TEXT("Wander");
}

EBTNodeResult::Type UWanderTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn);
	if (!NavSys) return EBTNodeResult::Failed;

	FNavLocation RandomLocation;
	if (!NavSys->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), WanderRadius, RandomLocation))
		return EBTNodeResult::Failed;

	AIController->MoveToLocation(RandomLocation.Location, 5.0f);
	return EBTNodeResult::Succeeded;
}
