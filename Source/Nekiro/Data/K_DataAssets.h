// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "K_DataAssets.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class NEKIRO_API UK_CombatData : public UDataAsset
{
	GENERATED_BODY()

public:
	//Player Damage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Damage|Player", meta=(ClampMin="0.0", ToolTip="Default Damage"))
	float DEFAULT_ATTACK_DAMAGE = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Damage|Player", meta=(ClampMin="0.0", ToolTip="Power Damage"))
	float STRONG_ATTACK_DAMAGE = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Damage|Player", meta=(ClampMin="0.0", ToolTip="Critical Damage Multiplier"))
	float CRITICAL_DAMAGE_MULTIPLIER = 1.5f;
	
	//Boss Damage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Damage|Boss", meta=(ClampMin="0.0"))
	float BOSS_DEFAULT_ATTACK_DAMAGE = 35.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Damage|Boss", meta=(ClampMin="0.0"))
	float BOSS_STRONG_ATTACK_DAMAGE = 60.0f;


	//Combo
	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = "NEKIRO|Combo" , meta = (ClampMin = "0.0" , ToolTip = "Combo Input Limitaion Time") )
	float COMBO_INPUT_LIMIT_TIME = 0.45f;

	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = "NEKIRO|Combo" , meta = (ClampMin = "0.0" , ToolTip = "Combo Reset Delay") )
	float COMBO_RESET_DELAY = 1.25f;

	//Defense
	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = "NEKIRO|Defense" , meta = (ClampMin = "0.0" , ToolTip = "Parry Success Time") )
	float PARRY_SUCCESS_TIME = 0.2f;

	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = "NEKIRO|Defense" , meta = (ClampMin = "0.0" , ToolTip = "Guard Damage Decrease Ratio") )
	float GUARD_DAMAGE_REDUCTION_RATIO = 0.6f;
};


UCLASS ( BlueprintType )
class NEKIRO_API UK_MovementData : public UDataAsset
{
	GENERATED_BODY ()

public:
	//Player Movement
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Movement|Player", meta = ( ClampMin = "0.0"))
	float WALK_SPEED = 200.0f;

	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Movement|Player", meta = ( ClampMin = "0.0"))
	float RUN_SPEED = 400.0f;

	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Movement|Player", meta = ( ClampMin = "0.0"))
	float CROUCH_SPEED = 150.0f;

	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Movement|Player", meta = ( ClampMin = "0.0"))
	float SPRINT_SPEED = 600.0f;

	//Boss Movement
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "NEKIRO|Movement|Boss", meta = ( ClampMin = "0.0"))
	float BOSS_WALK_SPEED = 150.0f;

	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = "NEKIRO|Movement|Boss" , meta = (ClampMin = "0.0") )
	float BOSS_RUN_SPEED = 300.0f;
};


UCLASS ( BlueprintType )
class NEKIRO_API UK_StatusData : public UDataAsset
{
	GENERATED_BODY ()

public:
	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = "NEKIRO|Status|Player" , meta = (ClampMin = "0.0") )
	float PLAYER_MAX_HEALTH = 100.0f;

	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = "NEKIRO|Status|Player" , meta = (ClampMin = "0.0") )
	float PLAYER_HEALTH_REGEN_RATE = 3.0f;

	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = "NEKIRO|Status|Boss" , meta = (ClampMin = "0.0") )
	float BOSS_MAX_HEALTH = 500.0f;

};
