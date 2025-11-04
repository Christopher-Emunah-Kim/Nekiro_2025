// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Nekiro/Character/K_Player.h"
#include "Nekiro/Character/K_Boss.h"
#include "Nekiro/Character/K_BossAIController.h"


UBTTask_TurnToTarget::UBTTask_TurnToTarget ()
{
	NodeName = TEXT ( "Turn To Target" );
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory )
{
	EBTNodeResult::Type result = Super::ExecuteTask ( OwnerComp , NodeMemory );

	auto boss = Cast<AK_Boss> ( OwnerComp.GetAIOwner ()->GetPawn () );
	if (!boss)
	{
		return EBTNodeResult::Failed;
	}

	auto target = Cast<AK_Player> ( OwnerComp.GetBlackboardComponent ()->GetValueAsObject ( AK_BossAIController::targetActorKey ) );
	if(!target)
	{
		return EBTNodeResult::Failed;
	}

	FVector dir = (target->GetActorLocation () - boss->GetActorLocation ()).GetSafeNormal ();
	dir.Z = 0.f;
	FRotator lookAtRotation = FRotationMatrix::MakeFromX ( dir ).Rotator ();

	boss->SetActorRotation ( FMath::RInterpTo ( boss->GetActorRotation () , lookAtRotation , GetWorld ()->GetDeltaSeconds () , 2.f ) );

	return EBTNodeResult::Succeeded;
}
