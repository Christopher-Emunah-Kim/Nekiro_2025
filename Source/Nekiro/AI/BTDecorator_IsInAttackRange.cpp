// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Nekiro/Character/K_BossAIController.h"
#include "Nekiro/Character/K_Player.h"
#include "Nekiro/Character/K_Boss.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange ()
{
	NodeName = TEXT ( "CanAttack" );
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory ) const
{
	bool bResult = Super::CalculateRawConditionValue ( OwnerComp , NodeMemory );

	auto pawn = Cast<AK_Boss>(OwnerComp.GetAIOwner ()->GetPawn ());
	if (!pawn)
	{
		return false;
	}

	auto target = Cast<AK_Player> ( OwnerComp.GetBlackboardComponent ()->GetValueAsObject ( AK_BossAIController::targetActorKey ) );
	if (!target)
	{
		return false;
	}

	bResult = (target->GetDistanceTo ( pawn ) <= pawn->GetBossAttackRange());
	return bResult;
}
