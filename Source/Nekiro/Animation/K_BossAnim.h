// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "K_BossAnimStates.h"

#include "K_BossAnim.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams ( FBossActionStateChangeDelegate , FName , actionName , bool , bIsActive );
DECLARE_DYNAMIC_MULTICAST_DELEGATE ( FOnBossAttackAnimEndDelegate );

class UAnimMontage;
/**
 * 
 */
UCLASS()
class NEKIRO_API UK_BossAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UK_BossAnim ();

	virtual void NativeInitializeAnimation () override;
	virtual void NativeUpdateAnimation ( float DeltaSeconds ) override;

protected:
	void CacheBossCharacter ();

	UFUNCTION()
	void HandleMontageEnded ( UAnimMontage* montage , bool bInterrupted );

public:
	UPROPERTY ( BlueprintAssignable , Category = "NEKIRO|Boss|Animation" )
	FBossActionStateChangeDelegate OnBossActionStateChangeDel;

	UPROPERTY ( BlueprintAssignable , Category = "NEKIRO|Boss|Animation" )
	FOnBossAttackAnimEndDelegate OnBossAttackAnimEndDel;

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Boss|Animation" )
	void PlayBossAttackMontage ( FName startSectionName );

	UFUNCTION()
	void AnimNotify_NSBossAttack ();

	void PlayBossDeathMontage ();
	void SetIsBossDead ( bool bDead );

private:
	TWeakObjectPtr<class AK_Boss> bossCharacter;

protected:
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Boss|Animation" )
	FBossAnimStates bossAnimStates;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Boss|Animation" )
	UAnimMontage* attackMontage;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Boss|Animation" )
	UAnimMontage* deathMontage;

};
