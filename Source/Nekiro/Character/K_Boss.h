// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Nekiro/Animation/K_BossAnimStates.h"
#include "K_Boss.generated.h"


DECLARE_MULTICAST_DELEGATE ( FOnBossAttackEndDelegate );

class UBehaviorTree;
class UBlackboardData;
class UStaticMeshComponent;
class UK_BossAnim;
class UK_ActionComp;
class UK_StatusComp;
class UK_MovementData;
class UK_CombatData;
class UWidgetComponent;

UCLASS()
class NEKIRO_API AK_Boss : public ACharacter
{
	GENERATED_BODY()

public:
	AK_Boss();

protected:
	virtual void BeginPlay() override;

	void InitializeComponents ();
	void BindAnimDelegateActions ();
	void BillboardBossHPUIToCamera ();

	UFUNCTION()
	void OnBossStateChanged (FName actionName , bool bIsActive );

	UFUNCTION()
	void OnAttackEnd ();

	UFUNCTION()
	void OnBossDeath ();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage ( float DamageAmount , struct FDamageEvent const& DamageEvent , AController* EventInstigator , AActor* DamageCauser ) override;

	FOnBossAttackEndDelegate OnBossAttackEndDel;


	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Boss|Combat" )
	void ReqeustAttack ( const FName& attackName );

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Boss|Combat" )
	void PerformAttack ();

	float GetBossAttackRange () const;

	//Getters
	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Boss|AI" )
	UBehaviorTree* GetBossBehaviorTree () const { return bossBehaviorTree; }

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Boss|AI" )
	UBlackboardData* GetBossBlackboardData () const { return bossBlackboardData; }

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Boss|AI" )
	UK_StatusComp* GetStatusComp () const { return statusComp; }

	UStaticMeshComponent* GetKatanaMeshComp () const { return katanaMeshComp; }

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

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|DataAssets" , meta = (AllowPrivateAccess = "true" , ToolTip = "Boss Combat Data Asset") )
	UK_CombatData* bossCombatData;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|UI" , meta = ( AllowPrivateAccess = "true" ) )
	UWidgetComponent* bossHPUIComp;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|UI" , meta = (AllowPrivateAccess = "true") )
	class UBossHPWidget* bossHPUI;
};
