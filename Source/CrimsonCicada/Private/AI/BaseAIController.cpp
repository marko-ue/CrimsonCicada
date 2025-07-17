

#include "AI/BaseAIController.h"

ABaseAIController::ABaseAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();
	
}


