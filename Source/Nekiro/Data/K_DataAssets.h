// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "K_DataAssets.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class NEKIRO_API UK_DamageData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Damage", meta=(ClampMin="0.0", ToolTip="Default Damage"))
	float defaultAttackDamage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Damage", meta=(ClampMin="0.0", ToolTip="Power Damage"))
	float powerAttackDamage = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Damage", meta=(ClampMin="0.0", ToolTip="Critical Damage Multiplier"))
	float ciriticalDamageMultiplier = 1.5f;
};


UCLASS(BlueprintType)
class NEKIRO_API UK_ComboData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Combo", meta=(ClampMin="0.0", ToolTip="Combo Input Limitaion Time"))
	float comboInputLimitTime = 0.45f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Combo", meta=(ClampMin="0.0", ToolTip="Combo Reset Delay"))
	float comboResetDelay = 1.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Combo", meta=(ClampMin="0.0", ToolTip="Combo Lockout TIme"))
	float comboLockoutTime = 0.2f; //마지막 콤보 후 추가입력 미허용 시간.
};


UCLASS ( BlueprintType )
class NEKIRO_API UK_DefenseData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Defense", meta = ( ClampMin = "0.0", ToolTip = "Parry Success Time" ) )
	float parrySuccessTime = 0.2f;

	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Defense", meta = ( ClampMin = "0.0", ToolTip = "Guard Damage Decrease Ratio"))
	float guardDamageDecreaseRatio = 0.6f;

};


UCLASS ( BlueprintType )
class NEKIRO_API UK_MovementData : public UDataAsset
{
	GENERATED_BODY ()

public:
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Movement", meta = ( ClampMin = "0.0", ToolTip = "Default Dash Distance"))
	float dashDistance = 600.0f;

	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = "NEKIRO|Movement" , meta = (ClampMin = "0.0" , ToolTip = "Default Dash Cooldown Time") )
	float dashCooldownTime = 1.0f;

	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = "NEKIRO|Movement" , meta = (ClampMin = "0.0" , ToolTip = "Roll Invincibility Time") )
	float rollInvincibilityTime = 0.3f; 
};


UCLASS ( BlueprintType )
class NEKIRO_API UK_PostureData : public UDataAsset
{
	GENERATED_BODY ()

public:
	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = "NEKIRO|Posture" , meta = (ClampMin = "0.0" , ToolTip = "Max Posture Gauge") )
	float maxPostureGauge = 100.0f;

	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = "NEKIRO|Posture" , meta = (ClampMin = "0.0" , ToolTip = "Posture Recovery Rate") )
	float postureRecoveryRate = 5.0f;

	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = "NEKIRO|Posture" , meta = (ClampMin = "0.0" , ToolTip = "Breaked Posture Rocovery Time") )
	float breakedPostureRecoveryTime = 3.f;

};