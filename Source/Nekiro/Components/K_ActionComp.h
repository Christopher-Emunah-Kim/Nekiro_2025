// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "K_ActionComp.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnGuardStateDelegate , bool , bIsGuarding );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnLockOnDelegate , bool , bIsLockOn );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams ( FOnAttackStateDelegate , bool , bIsAttacking , int32 , ComboIndex );


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEKIRO_API UK_ActionComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UK_ActionComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FOnAttackStateDelegate OnAttackStateDel;
	FOnGuardStateDelegate OnGuardStateDel;
	FOnLockOnDelegate OnTargetLockOnDel;

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Action" )
	void PerformAttack ();

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Action" )
	void StartGuard ();

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Action" )
	void CompleteGuard ();

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Action" )
	void StartLockOn ();

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Action" )
	void CompleteLockOn ();

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Action" )
	void SetAttackFlag ( bool bNewAttack ) { bIsAttacking = bNewAttack; }

	UFUNCTION(BlueprintCallable, Category = "NEKIRO|Action")
	void SetGuardFlag ( bool bNewGuard ) { bIsGuarding = bNewGuard; }

	UFUNCTION(BlueprintCallable, Category = "NEKIRO|Action")
	void SetLockOnFlag ( bool bNewLockOn ) { bIsLockOn = bNewLockOn; }

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Action" )
	bool IsAttacking () const { return bIsAttacking; }

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Action" )
	bool IsGuarding () const { return bIsGuarding; }

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Action" )
	bool IsLockOn () const { return bIsLockOn; }

protected:
	UPROPERTY ( VisibleAnywhere ,BlueprintReadOnly, Category = "NEKIRO|ActionComp|Components" )
	class AK_Player* playerOwner;

	UPROPERTY ( VisibleAnywhere ,BlueprintReadOnly, Category = "NEKIRO|ActionComp|Components" )
	class AK_Boss* targetBoss;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|ActionComp|DataAssets" , meta = (ToolTip = "Player Damage Data Asset") )
	class UK_CombatData* combatData;

	UPROPERTY ( VisibleAnywhere ,BlueprintReadOnly, Category = "NEKIRO|ActionComp|Components" )
	bool bIsAttacking = false;

	UPROPERTY ( VisibleAnywhere ,BlueprintReadOnly, Category = "NEKIRO|ActionComp|Components" )
	bool bIsGuarding = false;

	UPROPERTY ( VisibleAnywhere ,BlueprintReadOnly, Category = "NEKIRO|ActionComp|Components" )
	bool bIsLockOn = false;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|ActionComp|Components" )
	int32 currentComboIndex = 0;
};
