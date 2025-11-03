// Fill out your copyright notice in the Description page of Project Settings.


#include "K_BossAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "NEKIRO/Character/K_Boss.h"

const FName AK_BossAIController::currentPosKey = FName ( TEXT ( "currentPos" ) );
const FName AK_BossAIController::patrolPosKey = FName ( TEXT ( "patrolPos" ) );
const FName AK_BossAIController::targetActorKey = FName ( TEXT ( "targetActor" ) );



AK_BossAIController::AK_BossAIController ()
{
	repeatInterval = 3.0f;
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

	GetWorld()->GetTimerManager().SetTimer(repeatTimerHandle, this, &AK_BossAIController::OnRepeatTimer, repeatInterval, true);
}

void AK_BossAIController::OnUnPossess ()
{
	Super::OnUnPossess ();

	if (behaviorTreeComp)
	{
		behaviorTreeComp->StopTree ( EBTStopMode::Safe );
	}

	GetWorld()->GetTimerManager().ClearTimer(repeatTimerHandle);
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
			blackboardComp->SetValueAsVector ( currentPosKey , boss->GetActorLocation () );
		}

		RunBehaviorTree ( behaviorTree );
	}
}

void AK_BossAIController::OnRepeatTimer()
{
	auto pawn = GetPawn();
	if (pawn)
	{
		UNavigationSystemV1* navSys = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		if (!navSys)
		{
			return;
		}

		FNavLocation nextLocation;
		const bool bFound = navSys->GetRandomPointInNavigableRadius(pawn->GetActorLocation(), 500.0f, nextLocation);
		if (bFound)
		{
			//MoveTo
			UAIBlueprintHelperLibrary::SimpleMoveToLocation ( this , nextLocation.Location );
		}
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