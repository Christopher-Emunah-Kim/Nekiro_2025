// Fill out your copyright notice in the Description page of Project Settings.


#include "K_BossAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "NEKIRO/Character/K_Boss.h"

AK_BossAIController::AK_BossAIController ()
{
	targetActorName = FName ( TEXT ( "TargetActor" ) );
	
	behaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent> ( TEXT ( "BehaviorTreeComponent" ) );
	blackboardComp = CreateDefaultSubobject<UBlackboardComponent> ( TEXT ( "BlackboardComponent" ) );

}

void AK_BossAIController::OnPossess ( APawn* InPawn )
{
	Super::OnPossess ( InPawn );

	AK_Boss* boss = Cast<AK_Boss> ( InPawn );
	if (boss)
	{
		InitializeBehaviorTree ( boss );
	}
}

void AK_BossAIController::OnUnPossess ()
{
	Super::OnUnPossess ();

	if (behaviorTreeComp)
	{
		behaviorTreeComp->StopTree ( EBTStopMode::Safe );
	}
}

void AK_BossAIController::InitializeBehaviorTree ( AK_Boss* boss )
{
	if (!boss)
	{
		return;
	}

	UBehaviorTree* behaviorTree = boss->GetBossBehaviorTree ();
	if (behaviorTree)
	{
		UBlackboardData* blackBoard = boss->GetBossBlackboardData ();
		if (blackBoard)
		{
			blackboardComp->InitializeBlackboard ( *blackBoard );
		}

		RunBehaviorTree ( behaviorTree );
	}
}

void AK_BossAIController::SetTargetActor ( AActor* targetActor )
{
	if (!blackboardComp)
	{
		return;
	}

	blackboardComp->SetValueAsObject ( targetActorName , targetActor );
}

void AK_BossAIController::ClearTargetActor ()
{
	SetTargetActor ( nullptr );
}