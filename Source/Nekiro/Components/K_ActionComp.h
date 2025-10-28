// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "K_ActionComp.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnGuardStateDelegate , bool , bIsGuarding );
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

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Action" )
	void PerformAttack ();

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Action" )
	void StartGuard ();

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Action" )
	void CompleteGuard ();



	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Action" )
	void SetAttackFlag ( bool bNewAttack ) { bIsAttacking = bNewAttack; }

	UFUNCTION(BlueprintCallable, Category = "NEKIRO|Action")
	void SetGuardFlag ( bool bNewGuard ) { bIsGuarding = bNewGuard; }

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Action" )
	bool IsAttacking () const { return bIsAttacking; }

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Action" )
	bool IsGuarding () const { return bIsGuarding; }


private:
	UPROPERTY()
	class AK_Player* ownerPlayer;

	UPROPERTY()
	class UK_PlayerAnim* playerAnim;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|DataAssets" , meta = (AllowPrivateAccess = "true" , ToolTip = "Player Movement Data Asset") )
	class UK_MovementData* movementDataAsset;
	

	UPROPERTY ( VisibleAnywhere ,BlueprintReadOnly, Category = "NEKIRO|Components" , meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking = false;

	UPROPERTY ( VisibleAnywhere ,BlueprintReadOnly, Category = "NEKIRO|Components" , meta = (AllowPrivateAccess = "true"))
	bool bIsGuarding = false;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|Components" , meta = (AllowPrivateAccess = "true") )
	int32 currentComboIndex = 0;
};
