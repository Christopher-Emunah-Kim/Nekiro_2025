// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "K_PlayerAnimStates.h"

#include "K_PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class NEKIRO_API UK_PlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

	UK_PlayerAnim () = default;

public:
	virtual void NativeInitializeAnimation () override;
	virtual void NativeUpdateAnimation ( float DeltaSeconds ) override;

	void UpdateAnimStates ( float DeltaSeconds );
	void CalculateSpeedAndDirection ( float DeltaSeconds );
	void CheckPlayerStates ();

	void SetPlayerCharacter ( class AK_Player& player );
	void SetIsInAir ( const bool bInAir ) { m_playerAnimStates.bIsInAir = bInAir; }
	void SetIsAttack ( const bool bAttack ) { m_playerAnimStates.bIsAttack = bAttack; }
	void SetIsDead ( const bool bDead ) { m_playerAnimStates.bIsDead = bDead; }
	void SetIsHit ( const bool bHit ) { m_playerAnimStates.bIsHit = bHit; }
	void SetIsDash ( const bool bDash ) { m_playerAnimStates.bIsDash = bDash; }
	void SetIsGuard ( const bool bGuard ) { m_playerAnimStates.bIsGuard = bGuard; }
	void SetIsCrouch ( const bool bCrouch ) { m_playerAnimStates.bIsCrouch = bCrouch; }
	void SetMovementState ( const EPlayerMovementState state ) { m_playerAnimStates.movementState = state; }
	void SetCombatState ( const EPlayerCombatState state ) { m_playerAnimStates.combatState = state; }

protected:
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	FPlayerAnimStates m_playerAnimStates;

	UPROPERTY(Transient)
	TObjectPtr<class AK_Player> m_player;


};
