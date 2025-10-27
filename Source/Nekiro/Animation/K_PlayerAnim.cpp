// Fill out your copyright notice in the Description page of Project Settings.


#include "K_PlayerAnim.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Nekiro/Character/K_Player.h"

void UK_PlayerAnim::NativeInitializeAnimation ()
{
	m_player = Cast<AK_Player> ( TryGetPawnOwner () );
	//m_playerAnimStates = m_player->GetAnimState ();
}

void UK_PlayerAnim::NativeUpdateAnimation ( float DeltaSeconds )
{
	if ( !m_player )
	{
		m_player = Cast<AK_Player> ( TryGetPawnOwner () );
		//m_playerAnimStates = m_player->GetAnimState ();
	}

	if ( m_player )
	{
		const FPlayerAnimStates& playerAnimStates = m_player->GetAnimState ();

		m_playerAnimStates.direction = playerAnimStates.direction;
		m_playerAnimStates.speed = playerAnimStates.speed;
		m_playerAnimStates.bIsInAir = playerAnimStates.bIsInAir;
		m_playerAnimStates.bIsDead = playerAnimStates.bIsDead;
		m_playerAnimStates.bIsAttack = playerAnimStates.bIsAttack;
		m_playerAnimStates.bIsSkill = playerAnimStates.bIsSkill;
		m_playerAnimStates.bIsHit = playerAnimStates.bIsHit;
		m_playerAnimStates.bIsDash = playerAnimStates.bIsDash;
		m_playerAnimStates.bIsGuard = playerAnimStates.bIsGuard;
		m_playerAnimStates.bIsCrouch = playerAnimStates.bIsCrouch;
		m_playerAnimStates.movementState = playerAnimStates.movementState;
		m_playerAnimStates.combatState = playerAnimStates.combatState;
	}
}

void UK_PlayerAnim::SetPlayerCharacter ( AK_Player& player )
{
	m_player = &player;
}
