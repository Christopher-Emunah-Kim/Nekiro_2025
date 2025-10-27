
#pragma once

#include "CoreMinimal.h"

#include "K_PlayerAnimStates.generated.h"

UENUM ( BlueprintType )
enum class EPlayerMovementState : uint8
{
	IDLE UMETA ( DisplayName = "Idle" ) ,
	WALK UMETA ( DisplayName = "Walk" ) ,
	RUN UMETA ( DisplayName = "Run" ) ,
	JUMP UMETA ( DisplayName = "Jump" ),
	DASH UMETA ( DisplayName = "Dash" )
};

UENUM ( BlueprintType )
enum class EPlayerCombatState : uint8
{
	None UMETA ( DisplayName = "None" ) ,
	Attack UMETA ( DisplayName = "Attack" ) ,
	Guard UMETA ( DisplayName = "Guard" )
};



USTRUCT ( BlueprintType )
struct NEKIRO_API FPlayerAnimStates
{
	GENERATED_BODY ()

	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	float direction = 0.0f;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	float speed = 0.0f;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	bool bIsInAir;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	bool bIsDead;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	bool bIsAttack;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	bool bIsSkill;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	bool bIsHit;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	bool bIsDash;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	bool bIsGuard;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	bool bIsCrouch;

	UPROPERTY ( BlueprintReadOnly , Category = "NEKIRO|Animation" )
	EPlayerMovementState movementState = EPlayerMovementState::IDLE;

	UPROPERTY ( BlueprintReadOnly , Category = "NEKIRO|Animation" )
	EPlayerCombatState combatState = EPlayerCombatState::None;
};