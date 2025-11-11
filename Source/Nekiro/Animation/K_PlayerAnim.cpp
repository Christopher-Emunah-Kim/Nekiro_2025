// Fill out your copyright notice in the Description page of Project Settings.


#include "K_PlayerAnim.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Nekiro/Character/K_Player.h"
#include "Nekiro/Components/K_ActionComp.h"

void UK_PlayerAnim::NativeInitializeAnimation ()
{
	m_player = Cast<AK_Player> ( TryGetPawnOwner () );
	CheckComboAttackSections ();
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

	if(m_playerAnimStates.bIsDead)
	{
		return;
	}

	UpdateAnimStates (DeltaSeconds);
	CheckComboAttackSections ();

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

	if (!attackMontage)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "AttackMontage is nullptr!" ) );
	}

	CheckComboAttackSections ();

	const int32 maxIndex = attackSectionNames.Num () - 1;

	const int32 validIndex = FMath::Clamp ( comboIndex , 0 , maxIndex );

	FName selectSection = attackSectionNames.Num () > 0 ? attackSectionNames[validIndex] : FName ( "Attack3" );

	if (Montage_IsPlaying ( attackMontage ))
	{
		Montage_JumpToSection ( selectSection , attackMontage );
		//UE_LOG ( LogTemp , Warning , TEXT ( "UK_PlayerAnim::EnterAttatkState - Jumped to Section: %s" ) , *selectSection.ToString () );
	}
	else
	{
		Montage_Play ( attackMontage , 1.f );
		Montage_JumpToSection ( selectSection , attackMontage );
		//UE_LOG ( LogTemp , Warning , TEXT ( "UK_PlayerAnim::EnterAttatkState - Started and Jumped to Section: %s" ) , *selectSection.ToString () );
	}

	FOnMontageEnded endDel;
	endDel.BindUObject ( this , &UK_PlayerAnim::OnAttackMontageEnded );
	Montage_SetEndDelegate ( endDel , attackMontage );
}

void UK_PlayerAnim::ExitAttackState ()
{
	m_playerAnimStates.bIsAttack = false;
	m_playerAnimStates.combatState = EPlayerCombatState::None;
	m_playerAnimStates.movementState = EPlayerMovementState::IDLE;
}

void UK_PlayerAnim::OnAttackMontageEnded ( UAnimMontage* Montage , bool bInterrupted )
{
	//UE_LOG ( LogTemp , Warning , TEXT ( "UK_PlayerAnim::OnAttackMontageEnded called. Interrupted: %s" ) , bInterrupted ? TEXT ( "True" ) : TEXT ( "False" ) );
	if(Montage != attackMontage)
	{
		return;
	}

	if (bInterrupted)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "OnAttackMontageEnded - Montage was interrupted, not resetting" ) );

		return;
	}

	UK_ActionComp* actionComp = m_player->GetActionComp ();
	if (actionComp)
	{
		actionComp->HandleAttackFinished ( );
	}
}

void UK_PlayerAnim::OnDeathMontageEnded ( UAnimMontage* Montage , bool bInterrupted )
{
	if(Montage != deathMontage)
	{
		return;
	}

	m_player->ShowResultUI ( false );
}

void UK_PlayerAnim::AnimNotify_AttackHitCheck ()
{
	//UE_LOG ( LogTemp , Warning , TEXT ( "UK_PlayerAnim::AnimNotify_AttackHitCheck called." ) );

	if(!m_player)
	{
		return;
	}

	UK_ActionComp* actionComp = m_player->GetActionComp ();
	if (actionComp)
	{
		actionComp->HandleAttackHitCheck ();
	}

}

void UK_PlayerAnim::AnimNotify_NextAttackCheck ()
{
	//UE_LOG ( LogTemp , Warning , TEXT ( "UK_PlayerAnim::AnimNotify_NextAttackCheck called." ) );

	if (!m_player)
	{
		return;
	}

	UK_ActionComp* actionComp = m_player->GetActionComp ();
	if (actionComp)
	{
		actionComp->HandleNextAttackCheck ();
	}
}

void UK_PlayerAnim::PlayDashMontage()
{
	if(!dashMontage)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "DashMontage is nullptr!" ) );
		return;
	}

	Montage_Stop(0.2f);
	Montage_Play(dashMontage, 1.f);
}

void UK_PlayerAnim::PlayDeathMontage ()
{
	if(!deathMontage)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "DeathMontage is nullptr!" ) );
		return;
	}

	Montage_Stop ( 0.2f );
	Montage_Play ( deathMontage , 1.f );

	FOnMontageEnded endDelegate;
	endDelegate.BindUObject ( this , &UK_PlayerAnim::OnDeathMontageEnded );
	Montage_SetEndDelegate ( endDelegate , deathMontage );
}

void UK_PlayerAnim::SetIsDead ( bool bDead )
{
	m_playerAnimStates.bIsDead = bDead;
	if (bDead)
	{
		m_playerAnimStates.bIsAttack = false;
		m_playerAnimStates.bIsGuard = false;
		m_playerAnimStates.bIsDash = false;
		m_playerAnimStates.movementState = EPlayerMovementState::IDLE;
		m_playerAnimStates.combatState = EPlayerCombatState::None;
		m_playerAnimStates.speed = 0.f;
		m_playerAnimStates.direction = 0.f;
	}
}

void UK_PlayerAnim::CalculateSpeedAndDirection ( float DeltaSeconds )
{
	const UCharacterMovementComponent* moveComp = m_player->GetCharacterMovement ();

	const float maxSpeed = moveComp ? moveComp->GetMaxSpeed () : 600.f;

	FVector vel = m_player->GetVelocity ();
	vel.Z = 0.f;

	const FVector rawVel = m_player->GetActorTransform ().InverseTransformVectorNoScale ( vel );
	float rawSpeed = rawVel.Size ();

	const float followRate = 8.f;
	smoothSpeed = FMath::FInterpTo ( smoothSpeed , rawSpeed , DeltaSeconds , followRate );

	const float deadZone = 20.f;
	if(smoothSpeed < deadZone)
	{
		smoothSpeed = 0.f;
		m_playerAnimStates.speed = 0.f;
		m_playerAnimStates.direction = 0.f;
		return;
	}

	m_playerAnimStates.speed = FMath::Clamp ( smoothSpeed , 0.f , maxSpeed );

	FRotator targetRot = smoothRot;

	if (rawSpeed > 0.f)
	{
		const FVector rawDir = rawVel.GetSafeNormal2D ();
		targetRot = FRotationMatrix::MakeFromX ( rawDir ).Rotator ();

		float deltaYaw = FMath::FindDeltaAngleDegrees ( smoothRot.Yaw , targetRot.Yaw );
		targetRot.Yaw = smoothRot.Yaw + deltaYaw;
	}

	smoothRot = FMath::RInterpTo ( smoothRot , targetRot , DeltaSeconds , followRate );
	smoothRot.Yaw = FMath::UnwindDegrees ( smoothRot.Yaw );

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

void UK_PlayerAnim::CheckComboAttackSections ()
{
	if (!attackMontage)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "AttackMontage is nullptr!" ) );
		return;
	}

	attackSectionNames.Empty ();
	const int32 numSections = attackMontage->GetNumSections ();
	for ( int32 i = 0 ; i < numSections ; ++i )
	{
		FName sectionName = attackMontage->GetSectionName ( i );
		if ( sectionName.ToString ().StartsWith ( "Attack" ) )
		{
			attackSectionNames.Add ( sectionName );
		}
	}
}


void UK_PlayerAnim::SetPlayerCharacter ( AK_Player& player )
{
	m_player = &player;
}
