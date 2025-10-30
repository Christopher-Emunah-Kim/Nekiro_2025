// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "K_BossAnimStates.generated.h"
/**
 * 
 */

UENUM ( BlueprintType )
enum class EBossMovementState : uint8
{
	IDLE UMETA ( DisplayName = "Idle" ) ,
	WALK UMETA ( DisplayName = "Walk" ) ,
	RUN UMETA ( DisplayName = "Run" ) ,
	JUMP UMETA ( DisplayName = "Jump" ) ,
	DASH UMETA ( DisplayName = "Dash" )
};

UENUM ( BlueprintType )
enum class EBossCombatState : uint8
{
	None UMETA ( DisplayName = "None" ) ,
	Attack UMETA ( DisplayName = "Attack" ) ,
	Guard UMETA ( DisplayName = "Guard" )
};

USTRUCT ( BlueprintType )
struct NEKIRO_API FBossAnimStates
{
	GENERATED_BODY()

public:
	UPROPERTY ( EditAnywhere, BlueprintReadOnly , Category = "NEKIRO|BOSS|Animation" )
	float moveSpeed = 0.f;

	UPROPERTY ( EditAnywhere, BlueprintReadOnly , Category = "NEKIRO|BOSS|Animation" )
	bool bIsDead = false;

	UPROPERTY ( EditAnywhere, BlueprintReadOnly , Category = "NEKIRO|BOSS|Animation" )
	bool bIsAttack = false;

	UPROPERTY ( EditAnywhere, BlueprintReadOnly , Category = "NEKIRO|BOSS|Animation" )
	bool bIsFalling = false;

	UPROPERTY ( EditAnywhere, BlueprintReadOnly , Category = "NEKIRO|BOSS|Animation" )
	bool bIsHit = false;

	UPROPERTY ( EditAnywhere, BlueprintReadOnly , Category = "NEKIRO|BOSS|Animation" )
	FName actionName = NAME_None;

	UPROPERTY ( EditAnywhere, BlueprintReadOnly , Category = "NEKIRO|BOSS|Animation" )
	float playRate = 1.f;

	UPROPERTY ( BlueprintReadOnly , Category = "NEKIRO|BOSS|Animation" )
	EBossMovementState movementState = EBossMovementState::IDLE;

	UPROPERTY ( BlueprintReadOnly , Category = "NEKIRO|BOSS|Animation" )
	EBossCombatState combatState = EBossCombatState::None;

};
