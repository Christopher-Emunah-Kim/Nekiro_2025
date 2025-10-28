
#pragma once

#include "CoreMinimal.h"

#include "K_PlayerAnimStates.generated.h"

UENUM ( BlueprintType )
enum class EPlayerMovementState : uint8
{
	IDLE UMETA ( DisplayName = "Idle" ) ,
	WALK UMETA ( DisplayName = "Walk" ) ,
	RUN UMETA ( DisplayName = "Run" ) ,
	CROUCH UMETA ( DisplayName = "Crouch" ) ,
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
	bool bIsInAir = false;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	bool bIsDead = false;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	bool bIsAttack = false;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	bool bIsHit = false;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	bool bIsDash = false;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	bool bIsGuard = false;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	bool bIsCrouch = false;

	UPROPERTY ( BlueprintReadOnly , Category = "NEKIRO|Animation" )
	EPlayerMovementState movementState = EPlayerMovementState::IDLE;

	UPROPERTY ( BlueprintReadOnly , Category = "NEKIRO|Animation" )
	EPlayerCombatState combatState = EPlayerCombatState::None;



	/*void SetSpeed ( const float spd ) { speed = spd; }
	void SetDirection ( const float dir ) { direction = dir; }
	void SetIsInAir ( const bool bInAir ) { bIsInAir = bInAir; }
	void SetIsDead ( const bool bDead ) { bIsDead = bDead; }
	void SetIsAttack ( const bool bAttack ) { bIsAttack = bAttack; }
	void SetIsHit ( const bool bHit ) { bIsHit = bHit; }
	void SetIsDash ( const bool bDash ) { bIsDash = bDash; }
	void SetIsGuard ( const bool bGuard ) { bIsGuard = bGuard; }
	void SetIsCrouch ( const bool bCrouch ) { bIsCrouch = bCrouch; }
	void SetMovementState ( const EPlayerMovementState state ) { movementState = state; }
	void SetCombatState ( const EPlayerCombatState state ) { combatState = state; }*/

};