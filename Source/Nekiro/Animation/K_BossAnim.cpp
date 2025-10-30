// Fill out your copyright notice in the Description page of Project Settings.


#include "K_BossAnim.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NEKIRO/Character/K_Boss.h"


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

	CacheBossCharacter ();

	if(!bossCharacter.IsValid())
	{
		bossAnimStates.moveSpeed = 0.f;
		bossAnimStates.bIsFalling = false;
		bossAnimStates.bIsDead = true;
		return;
	}

	const UCharacterMovementComponent* movementComp = bossCharacter->GetCharacterMovement ();
	if (movementComp)
	{
		bossAnimStates.moveSpeed = bossCharacter->GetVelocity ().Size2D ();
		bossAnimStates.bIsFalling = movementComp->IsFalling ();
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
		AK_Boss* castedBoss = Cast<AK_Boss> ( pawnOwner );
		if (castedBoss)
		{
			bossCharacter = castedBoss;
		}
	}
}

void UK_BossAnim::HandleMontageEnded ( UAnimMontage* montage , bool bInterrupted )
{
	OnBossActionStateChangeDel.Broadcast ( bossAnimStates.actionName , false );
	bossAnimStates.actionName = NAME_None;
}


void UK_BossAnim::PlayBossActionMontage ( UAnimMontage* montageToPlay, FName startSectionName )
{
	if (! montageToPlay )
	{
		return;
	}

	//update section name
	if (startSectionName.IsNone ())
	{
		startSectionName = montageToPlay->GetFName ();
	}

	bossAnimStates.actionName = startSectionName;

	//stop and play
	Montage_Stop ( 0.1f );

	Montage_Play ( montageToPlay, bossAnimStates.playRate );

	if (startSectionName != NAME_None)
	{
		Montage_JumpToSection ( startSectionName , montageToPlay );
	}

	//bind end delegate
	FOnMontageEnded endDelegate;
	endDelegate.BindUObject ( this , &UK_BossAnim::HandleMontageEnded);

	//execute delegate when montage ends
	Montage_SetEndDelegate ( endDelegate , montageToPlay );

	//broadcast boss action change
	OnBossActionStateChangeDel.Broadcast ( bossAnimStates.actionName , true );
}