// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"

#include "Nekiro/Character/K_BossAIController.h"
#include "Nekiro/Character/K_Boss.h"
#include "Nekiro/Character/K_Player.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

UBTService_Detect::UBTService_Detect ()
{
	NodeName = TEXT ( "Detect" );
	Interval = 1.0f;
}

void UBTService_Detect::TickNode ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory , float DeltaSeconds )
{
	Super::TickNode ( OwnerComp , NodeMemory , DeltaSeconds );

	PerformDetect ( OwnerComp);
}

void UBTService_Detect::PerformDetect ( UBehaviorTreeComponent& OwnerComp  )
{
	APawn* controlPawn = OwnerComp.GetAIOwner ()->GetPawn ();
	if (!controlPawn)
	{
		return;
	}

	UWorld* world = controlPawn->GetWorld ();

	if (!world)
	{
		return;
	}


	FVector pawnLocation = controlPawn->GetActorLocation ();
	float detectRadius = 1500.0f;

	TArray<AActor*> outActors;
	TArray<AActor*> ignoreActors{ controlPawn };

	//pawn만 검출
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add ( UEngineTypes::ConvertToObjectType ( ECC_Pawn ) );

	const bool bResult = UKismetSystemLibrary::SphereOverlapActors ( world , pawnLocation , detectRadius ,
		objectTypes , AActor::StaticClass () , ignoreActors , outActors );



	if (!bResult)
	{
		return;
	}

	OwnerComp.GetBlackboardComponent ()->SetValueAsObject ( AK_BossAIController::targetActorKey , nullptr );
	for (AActor* actor : outActors)
	{
		AK_Player* player = Cast<AK_Player> ( actor );
		if (player /*  && !player->IsDead () */) // TODO : 죽음 상태 체크
		{
			OwnerComp.GetBlackboardComponent ()->SetValueAsObject ( AK_BossAIController::targetActorKey , player );

			//DrawDebugSphere ( world , pawnLocation , detectRadius , 16 , FColor::Green , false , Interval );
			//DrawDebugPoint ( world , player->GetActorLocation () , 10.0f , FColor::Blue , false , Interval );
			//DrawDebugLine ( world , pawnLocation , player->GetActorLocation () , FColor::Blue , false , Interval );

			break;
		}
	}

	//DrawDebugSphere ( world , pawnLocation , detectRadius , 16 , FColor::Red , false , Interval );
}

