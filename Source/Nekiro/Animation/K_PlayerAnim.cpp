// Fill out your copyright notice in the Description page of Project Settings.


#include "K_PlayerAnim.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Nekiro/Character/K_Player.h"

void UK_PlayerAnim::NativeInitializeAnimation ()
{
	m_player = Cast<AK_Player> ( TryGetPawnOwner () );
	m_playerAnimStates = m_player->GetAnimState ();
}

void UK_PlayerAnim::NativeUpdateAnimation ( float DeltaSeconds )
{
	if ( !m_player )
	{
		m_player = Cast<AK_Player> ( TryGetPawnOwner () );
		m_playerAnimStates = m_player->GetAnimState ();
	}

	if ( m_player )
	{
		m_playerAnimStates = m_player->GetAnimState ();
	}
}

void UK_PlayerAnim::SetPlayerCharacter ( AK_Player& player )
{
	m_player = &player;
}
