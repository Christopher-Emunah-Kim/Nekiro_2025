// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "K_BossAIController.generated.h"

/**
 * 
 */

class AK_BossCharacter;

UCLASS()
class NEKIRO_API AK_BossAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AK_BossAIController ();

protected:
	virtual void OnPossess ( APawn* InPawn ) override;
	virtual void OnUnPossess () override;

	void InitializeBlackboardAndBehaviorTree ( AK_BossCharacter* boss );

public:
	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|BOSS|AI" )
	void SetTargetActor ( AActor* targetActor );

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|BOSS|AI" )
	void ClearTargetActor ();

protected:
	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|BOSS|AI" )
	class UBehaviorTreeComponent* behaviorTreeComp;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|BOSS|AI" )
	class UBlackboardComponent* blackboardComp;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|BOSS|AI" )
	FName targetActorName;

};
