// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "K_BossAnimStates.h"

#include "K_BossAnim.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams ( FBossActionStateChangeDelegate , FName , actionName , bool , bIsActive );

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

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Boss|Animation" )
	void PlayBossActionMontage ( UAnimMontage* montageToPlay, FName startSectionName );

private:
	TWeakObjectPtr<class AK_Boss> bossCharacter;

protected:
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Boss|Animation" )
	FBossAnimStates bossAnimStates;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Boss|Animation" )
	UAnimMontage* attackMontage;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Boss|Animation" )
	TMap<FName , UAnimMontage*> skillMontageMap;

};
