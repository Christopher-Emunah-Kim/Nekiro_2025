// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Nekiro/Animation/K_BossAnimStates.h"
#include "K_Boss.generated.h"

class UBehaviorTree;
class UBlackboardData;
class UStaticMeshComponent;
class UK_BossAnim;
class UK_ActionComp;
class UK_StatusComp;
class UK_MovementData;

UCLASS()
class NEKIRO_API AK_Boss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AK_Boss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBossStateChanged (FName actionName , bool bIsActive );

	void BindDelegateActions ();



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Boss|Combat" )
	void ReqeustAttack ( const FName& attackName );

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Boss|Combat" )
	void ReqeustSkill ( const FName& skillName );



	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Boss|AI" )
	UBehaviorTree* GetBossBehaviorTree () const { return bossBehaviorTree; }

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Boss|AI" )
	UBlackboardData* GetBossBlackboardData () const { return bossBlackboardData; }

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Boss|AI" )
	UK_StatusComp* GetStatusComp () const { return statusComp; }

protected:

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|BT" , meta = (AllowPrivateAccess = "true") )
	UBehaviorTree* bossBehaviorTree;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|BT" , meta = (AllowPrivateAccess = "true") )
	UBlackboardData* bossBlackboardData;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|Animation" , meta = ( AllowPrivateAccess = "true" ) )
	UK_BossAnim* bossAnim;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|Animation" , meta = ( AllowPrivateAccess = "true" ) )
	FBossAnimStates bossAnimStates;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|Component" , meta = ( AllowPrivateAccess = "true" ) )
	UK_StatusComp* statusComp;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|Component" , meta = ( AllowPrivateAccess = "true" ) )
	UStaticMeshComponent* katanaMeshComp;


	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|DataAssets" , meta = (AllowPrivateAccess = "true" , ToolTip = "Boss Movement Data Asset") )
	UK_MovementData* movementData;


};
