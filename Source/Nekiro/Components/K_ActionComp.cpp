// Fill out your copyright notice in the Description page of Project Settings.

#include "K_ActionComp.h"
#include "NEKIRO/Character/K_PlayerController.h"
#include "NEKIRO/Character/K_Player.h"
#include "NEKIRO/Animation/K_PlayerAnim.h"
#include "Nekiro/Character/K_Boss.h"
#include "GameFramework/CharacterMovementComponent.h"

#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>


// Sets default values for this component's properties
UK_ActionComp::UK_ActionComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UK_ActionComp::BeginPlay()
{
	Super::BeginPlay();

	playerOwner = Cast<AK_Player> ( GetOwner () );
}


// Called every frame
void UK_ActionComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (bIsLockOn && playerOwner && targetBoss && IsValid(targetBoss))
	{
		AK_PlayerController* pc = Cast<AK_PlayerController> ( playerOwner->GetController () );
		if (!pc)
		{
			return;
		}
		
		FVector playerLoc = playerOwner->GetActorLocation ();
		FVector targetLoc = targetBoss->GetActorLocation ();

		//lock-off when too far
		float dist = FVector::Distance ( playerLoc , targetLoc );
		if(dist > 3000.f)
		{
			CompleteLockOn ();
			return;
		}

		FRotator lookAtRot = UKismetMathLibrary::FindLookAtRotation ( playerLoc , targetLoc );
		FRotator currentRot = pc->GetControlRotation ();

		FRotator newRot = FMath::RInterpTo ( currentRot , lookAtRot , DeltaTime , 15.f );
		pc->SetControlRotation ( newRot );
	}
}


void UK_ActionComp::PerformAttack()
{
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
	bIsGuarding = true;

	OnGuardStateDel.Broadcast ( bIsGuarding );

		//TODO : 이후 패링 체크 로직
	
}

void UK_ActionComp::CompleteGuard()
{
	bIsGuarding = false;

	OnGuardStateDel.Broadcast ( bIsGuarding );
}

void UK_ActionComp::StartLockOn ()
{
	if(bIsLockOn)
	{
		return;
	}

	//UKismetSystemLibrary::SphereTraceSingleForObjects로 타겟(K_Boss 타입) 찾아서 카메라 락온하기
	AK_Player* player = Cast<AK_Player> ( GetOwner () );
	if (!player)
	{
		return;
	}

	APlayerCameraManager* pcm = UGameplayStatics::GetPlayerCameraManager ( GetWorld () , 0 );
	if(!pcm)
	{
		return;
	}

	FVector start = pcm->GetCameraLocation ();
	start = FVector ( start.X , start.Y , start.Z + 20.0f );

	FRotator cameraRot = pcm->GetCameraRotation ();
	FVector end = start + (cameraRot.Vector () * 5000.f);

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add ( EObjectTypeQuery::ObjectTypeQuery3 ); //Pawn Object Type
	//objectTypes.Add ( EObjectTypeQuery::ObjectTypeQuery1 ); //CharacterObjectType
	TArray<AActor*> ignoreActors;
	ignoreActors.Add ( player );

	FHitResult outHit;
	bool bIsHit = UKismetSystemLibrary::SphereTraceSingleForObjects ( GetWorld () , 
		start , end , 30.f , 
		objectTypes , false , ignoreActors , 
		EDrawDebugTrace::ForDuration , outHit , true,
		FLinearColor::Red, FLinearColor::Green, 5.0f);

	if (bIsHit)
	{
		AK_Boss* lockOnTarget = Cast<AK_Boss> ( outHit.GetActor () );
		if (lockOnTarget)
		{
			targetBoss = lockOnTarget;
			bIsLockOn = true;
			OnTargetLockOnDel.Broadcast ( bIsLockOn );

			UE_LOG ( LogTemp , Warning , TEXT ( "Lock-On Target : %s" ) , *targetBoss->GetName () );
			return;
		}
		else
		{
			UE_LOG ( LogTemp , Warning , TEXT ( "Lock-On Target is not Boss Type" ) );
		}
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "No Lock-On Hit Detected" ) );
	}

	//락온 실패
	{
		bIsLockOn = false;
		OnTargetLockOnDel.Broadcast ( bIsLockOn );
		UE_LOG ( LogTemp , Warning , TEXT ( "Lock-On Failed" ) );
		return;
	}
}

void UK_ActionComp::CompleteLockOn ()
{
	if(!bIsLockOn)
	{
		return;
	}

	bIsLockOn = false;
	targetBoss = nullptr;
	OnTargetLockOnDel.Broadcast ( bIsLockOn );

	UE_LOG ( LogTemp , Warning , TEXT ( "Lock-On Released" ) );
}
