// Fill out your copyright notice in the Description page of Project Settings.


#include "K_PlayerAnim.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Nekiro/Character/K_Player.h"
#include "Nekiro/Components/K_ActionComp.h"

void UK_PlayerAnim::NativeInitializeAnimation ()
{
	m_player = Cast<AK_Player> ( TryGetPawnOwner () );
}

void UK_PlayerAnim::NativeUpdateAnimation ( float DeltaSeconds )
{
	if ( !m_player )
	{
		m_player = Cast<AK_Player> ( TryGetPawnOwner () );
		if (!m_player)
		{
			return;
		}
	}

	UpdateAnimStates (DeltaSeconds);

}

void UK_PlayerAnim::UpdateAnimStates ( float DeltaSeconds )
{
	CalculateSpeedAndDirection ( DeltaSeconds );

	CheckPlayerStates ();
}

void UK_PlayerAnim::EnterGuardState ()
{
	m_playerAnimStates.bIsGuard = true;
	m_playerAnimStates.bIsAttack = false;
	m_playerAnimStates.combatState = EPlayerCombatState::Guard;
}

void UK_PlayerAnim::ExitGuardState ()
{
	m_playerAnimStates.bIsGuard = false;
	m_playerAnimStates.combatState = EPlayerCombatState::None;
	m_playerAnimStates.movementState = EPlayerMovementState::IDLE;
}

void UK_PlayerAnim::EnterAttatkState ( int32 comboIndex )
{
	m_playerAnimStates.bIsAttack = true;
	m_playerAnimStates.combatState = EPlayerCombatState::Attack;

	if (attackMontage)
	{
		Montage_Play ( attackMontage , 1.f);
		FName selectSection = FName ( "Attack3" );
		Montage_JumpToSection ( selectSection , attackMontage );
	}
}

void UK_PlayerAnim::ExitAttackState ()
{
	m_playerAnimStates.bIsAttack = false;
	m_playerAnimStates.combatState = EPlayerCombatState::None;
	m_playerAnimStates.movementState = EPlayerMovementState::IDLE;
}

void UK_PlayerAnim::AnimNotify_AttackHitCheck ()
{
}

void UK_PlayerAnim::AnimNotify_NextAttackCheck ()
{
}

void UK_PlayerAnim::CalculateSpeedAndDirection ( float DeltaSeconds )
{
	const UCharacterMovementComponent* moveComp = m_player->GetCharacterMovement ();

	const float maxSpeed = moveComp ? moveComp->GetMaxSpeed () : 600.f;

	FVector vel = m_player->GetVelocity ();
	vel.Z = 0.f;

	const FVector rawVel = m_player->GetActorTransform ().InverseTransformVectorNoScale ( vel );
	float rawSpeed = rawVel.Size ();

	const float deadZone = 3.f;
	if (rawSpeed < deadZone)
	{
		rawSpeed = 0.f;
	}

	const float followRate = 8.f;
	static float smoothSpeed = 0.f;
	smoothSpeed = FMath::FInterpTo ( smoothSpeed , rawSpeed , DeltaSeconds , followRate );

	m_playerAnimStates.speed = FMath::Clamp ( smoothSpeed , 0.f , maxSpeed );

	static FRotator smoothRot = FRotator::ZeroRotator;
	FRotator targetRot = smoothRot;

	if (rawSpeed > 0.f)
	{
		const FVector rawDir = rawVel.GetSafeNormal2D ();
		targetRot = FRotationMatrix::MakeFromX ( rawDir ).Rotator ();
	}

	smoothRot = FMath::RInterpTo ( smoothRot , targetRot , DeltaSeconds , followRate );
	m_playerAnimStates.direction = FRotator::NormalizeAxis ( smoothRot.Yaw );
}

void UK_PlayerAnim::CheckPlayerStates ()
{
	const UCharacterMovementComponent* moveComp = m_player->GetCharacterMovement ();
	if (moveComp)
	{
		m_playerAnimStates.bIsInAir = moveComp->IsFalling ();
		//m_playerAnimStates.bIsCrouch = moveComp->IsCrouching ();
	}
	bool isInAir = m_playerAnimStates.bIsInAir;
	if (isInAir)
	{
		m_playerAnimStates.movementState = EPlayerMovementState::JUMP;
	}
	else if (FMath::Abs ( m_playerAnimStates.speed ) > 200.0f || FMath::Abs ( m_playerAnimStates.direction ) > 200.0f)
	{
		m_playerAnimStates.movementState = EPlayerMovementState::RUN;
	}
	else if (!FMath::IsNearlyZero ( m_playerAnimStates.speed ) || !FMath::IsNearlyZero ( m_playerAnimStates.direction ))
	{
		m_playerAnimStates.movementState = EPlayerMovementState::WALK;
	}
	else
	{
		m_playerAnimStates.movementState = EPlayerMovementState::IDLE;
	}

	//if (moveComp && moveComp->IsCrouching ())
	if (m_playerAnimStates.bIsCrouch)
	{
		m_playerAnimStates.movementState = EPlayerMovementState::CROUCH;
	}

	const UK_ActionComp* actionComp = m_player->GetActionComp ();
	const bool isAttacking = actionComp && actionComp->IsAttacking ();
	m_playerAnimStates.bIsAttack = isAttacking;
	if (isAttacking)
	{
		m_playerAnimStates.combatState = EPlayerCombatState::Attack;
	}
	else if(m_playerAnimStates.bIsGuard)
	{
		m_playerAnimStates.combatState = EPlayerCombatState::Guard;
	}
	else
	{
		m_playerAnimStates.combatState = EPlayerCombatState::None;
	}
}


void UK_PlayerAnim::SetPlayerCharacter ( AK_Player& player )
{
	m_player = &player;
}
