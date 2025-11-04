// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "Nekiro/Character/K_Boss.h"
#include "Nekiro/Character/K_BossAIController.h"

UBTTask_Attack::UBTTask_Attack ()
{
	bNotifyTick = true;
	bIsAttacking = false;
	attackNames = { TEXT ( "1" ) , TEXT ( "1_r" ) , TEXT ( "2" ) , TEXT ( "2_r" ) };
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory )
{
	EBTNodeResult::Type result = Super::ExecuteTask ( OwnerComp , NodeMemory );

	AK_Boss* boss = Cast<AK_Boss> ( OwnerComp.GetAIOwner ()->GetPawn () );
	if (!boss)
	{
		return EBTNodeResult::Failed;
	}
	
	FName randName = rand () % 3 == 0 ? attackNames[0] : rand () % 3 == 1 ? attackNames[1] : rand () % 3 == 2 ? attackNames[2] : attackNames[3];
	boss->ReqeustAttack ( randName );

	bIsAttacking = true;
	boss->OnBossAttackEndDel.AddLambda ( [this]() 
		{
			bIsAttacking = false;
		} );

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory , float DeltaSeconds )
{
	Super::TickTask ( OwnerComp , NodeMemory , DeltaSeconds );

	if (!bIsAttacking)
	{
		FinishLatentTask ( OwnerComp , EBTNodeResult::Succeeded );
	}
}
