// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Nekiro/Character/K_BossAIController.h"

#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


UBTTask_FindPatrolPos::UBTTask_FindPatrolPos ()
{
	NodeName = TEXT ( "Find Patrol Position" );
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory )
{
	EBTNodeResult::Type result = Super::ExecuteTask ( OwnerComp , NodeMemory );

	auto pawn = OwnerComp.GetAIOwner ()->GetPawn ();
	if (!pawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* navSys = UNavigationSystemV1::GetNavigationSystem ( pawn->GetWorld());
	if(!navSys)
	{
		return EBTNodeResult::Failed;
	}

	FVector origin = OwnerComp.GetBlackboardComponent ()->GetValueAsVector ( AK_BossAIController::currentPosKey );

	FNavLocation nextPatrol;

	if(navSys->GetRandomPointInNavigableRadius ( origin , 500.0f , nextPatrol ))
	{
		OwnerComp.GetBlackboardComponent ()->SetValueAsVector ( AK_BossAIController::patrolPosKey , nextPatrol.Location );
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
