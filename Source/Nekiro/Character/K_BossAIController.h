// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TimerManager.h"

#include "K_BossAIController.generated.h"

/**
 * 
 */

class AK_Boss;
class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS()
class NEKIRO_API AK_BossAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AK_BossAIController ();

protected:
	virtual void OnPossess ( APawn* InPawn ) override;
	virtual void OnUnPossess () override;

	void InitializeBehaviorTree ( AK_Boss* boss );
	void OnRepeatTimer ();

public:
	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|BOSS|AI" )
	void SetTargetActor ( AActor* targetActor );

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|BOSS|AI" )
	void ClearTargetActor ();





protected:
	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|BOSS|AI" )
	UBehaviorTreeComponent* behaviorTreeComp;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|BOSS|AI" )
	UBlackboardComponent* blackboardComp;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|BOSS|AI" )
	FName targetActorName;

	FTimerHandle repeatTimerHandle;
	float repeatInterval;

public:
	static const FName currentPosKey;
	static const FName patrolPosKey;
	static const FName targetActorKey;

};
