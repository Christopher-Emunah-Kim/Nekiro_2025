// Fill out your copyright notice in the Description page of Project Settings.

#include "K_ActionComp.h"
#include "NEKIRO/Character/K_PlayerController.h"
#include "NEKIRO/Character/K_Player.h"
#include "NEKIRO/Animation/K_PlayerAnim.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UK_ActionComp::UK_ActionComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ownerPlayer = Cast<AK_Player>(GetOwner());
}


// Called when the game starts
void UK_ActionComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (!ownerPlayer)
	{
		ownerPlayer = Cast<AK_Player> ( GetOwner () );
	}

	if (!playerAnim)
	{
		playerAnim = Cast<UK_PlayerAnim> ( ownerPlayer->GetMesh ()->GetAnimInstance () );
	}
}


// Called every frame
void UK_ActionComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UK_ActionComp::PerformAttack()
{
	if (!ownerPlayer)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Owner Player is Nullptr" ) );
		return;
	}

	if (bIsAttacking)
	{
		//콤보처리
		currentComboIndex++;
	}
	else
	{
		currentComboIndex = 0;
	}

	bIsAttacking = true;

	OnAttackStateDel.Broadcast ( bIsAttacking , currentComboIndex );
}


void UK_ActionComp::StartGuard()
{
	if (!ownerPlayer)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Owner Player is Nullptr" ) );
		return;
	}

	bIsGuarding = true;

	OnGuardStateDel.Broadcast ( bIsGuarding );

		//TODO : 이후 패링 체크 로직
	
}

void UK_ActionComp::CompleteGuard()
{
	if (!ownerPlayer)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Owner Player is Nullptr" ) );
		return;
	}

	bIsGuarding = false;

	OnGuardStateDel.Broadcast ( bIsGuarding );
}

