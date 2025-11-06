// Fill out your copyright notice in the Description page of Project Settings.


#include "K_BossAnim.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"

#include "NEKIRO/Character/K_Boss.h"
#include "Nekiro/Character/K_Player.h"

#include <Kismet/GameplayStatics.h>


UK_BossAnim::UK_BossAnim ()
{
}

void UK_BossAnim::NativeInitializeAnimation ()
{
	Super::NativeInitializeAnimation ();
	// Initialization logic here

	CacheBossCharacter ();
}

void UK_BossAnim::NativeUpdateAnimation ( float DeltaSeconds )
{
	Super::NativeUpdateAnimation ( DeltaSeconds );
	// Update logic here

	if (!bossCharacter.IsValid ())
	{
		CacheBossCharacter ();
	}

	if(!bossCharacter.IsValid())
	{
		bossAnimStates.bossSpeed = 0.f;
		bossAnimStates.bIsFalling = false;
		return;
	}

	if (bossAnimStates.bIsDead)
	{
		bossAnimStates.bossSpeed = 0.f;
		bossAnimStates.bIsFalling = false;
		return;
	}

	const UCharacterMovementComponent* movementComp = bossCharacter->GetCharacterMovement ();
	if (movementComp)
	{
		bossAnimStates.bossSpeed = bossCharacter->GetVelocity ().Size2D ();
		bossAnimStates.bIsFalling = movementComp->IsFalling ();
		if (bossAnimStates.bossSpeed > 200.f)
		{
			int a = 0;
		}
	}
}

void UK_BossAnim::CacheBossCharacter ()
{
	if (bossCharacter.IsValid ())
	{
		return;
	}

	APawn* pawnOwner = TryGetPawnOwner ();
	if (pawnOwner)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "PawnOwner: %s, Class: %s" ) ,
			*pawnOwner->GetName () ,
			*pawnOwner->GetClass ()->GetName () );

		AK_Boss* castedBoss = Cast<AK_Boss> ( pawnOwner );
		if (castedBoss)
		{
			bossCharacter = castedBoss;
			UE_LOG ( LogTemp , Warning , TEXT ( "Boss Character Cached Successfully!" ) );
		}
		else
		{
			UE_LOG ( LogTemp , Error , TEXT ( "Failed to cast PawnOwner to AK_Boss!" ) );
		}
	}
	else
	{
		UE_LOG ( LogTemp , Error , TEXT ( "PawnOwner is null!" ) );
	}
}

void UK_BossAnim::HandleMontageEnded ( UAnimMontage* montage , bool bInterrupted )
{
	OnBossActionStateChangeDel.Broadcast ( bossAnimStates.actionName , false );
	bossAnimStates.actionName = NAME_None;

	if (montage == attackMontage)
	{
		OnBossAttackAnimEndDel.Broadcast ();
	}

	if(montage == deathMontage)
	{
		AK_Player* player = Cast<AK_Player> ( UGameplayStatics::GetPlayerCharacter ( GetWorld () , 0 ) );
		if (player)
		{
			player->ShowResultUI ( true );
		}
	}
}


void UK_BossAnim::PlayBossAttackMontage (  FName startSectionName )
{
	if(!attackMontage)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "No Attack Montage found!" ) );
		return;
	}

	//update section name
	if (startSectionName.IsNone ())
	{
		startSectionName = attackMontage->GetFName ();
	}

	bossAnimStates.actionName = startSectionName;

	//stop and play
	Montage_Stop ( 0.1f );

	Montage_Play ( attackMontage , bossAnimStates.playRate );

	if (startSectionName != NAME_None)
	{
		Montage_JumpToSection ( startSectionName , attackMontage );
	}

	//bind end delegate
	FOnMontageEnded endDelegate;
	endDelegate.BindUObject ( this , &UK_BossAnim::HandleMontageEnded);

	//execute delegate when montage ends
	Montage_SetEndDelegate ( endDelegate , attackMontage );

	//broadcast boss action change
	OnBossActionStateChangeDel.Broadcast ( bossAnimStates.actionName , true );
}

void UK_BossAnim::AnimNotify_NSBossAttack ()
{
	bossCharacter->PerformAttack ();

	UE_LOG ( LogTemp , Warning , TEXT ( "Boss Attack Notify Triggered!" ) );
}

void UK_BossAnim::PlayBossDeathMontage ()
{
	if (!bossCharacter.IsValid ())
	{
		return;
	}

	if(!deathMontage)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "No Death Montage found!" ) );
		return;
	}

	Montage_Stop ( 0.2f );
	Montage_Play ( deathMontage , 1.f );
	Montage_JumpToSection ( FName("RemoveLife") , deathMontage );
	

	FOnMontageEnded endDelegate;
	endDelegate.BindUObject ( this , &UK_BossAnim::HandleMontageEnded );
	Montage_SetEndDelegate ( endDelegate , deathMontage );
}

void UK_BossAnim::SetIsBossDead ( bool bDead )
{
	bossAnimStates.bIsDead = bDead;
	if (bDead)
	{
		bossAnimStates.bIsAttack = false;
		bossAnimStates.movementState = EBossMovementState::IDLE;
		bossAnimStates.combatState = EBossCombatState::None;
		bossAnimStates.bossSpeed = 0.f;
		bossAnimStates.bIsFalling = false;
		bossAnimStates.actionName = NAME_None;
	}
}