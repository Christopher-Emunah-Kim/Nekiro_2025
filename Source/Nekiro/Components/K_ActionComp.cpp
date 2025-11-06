// Fill out your copyright notice in the Description page of Project Settings.

#include "K_ActionComp.h"
#include "NEKIRO/Character/K_PlayerController.h"
#include "NEKIRO/Character/K_Player.h"
#include "NEKIRO/Animation/K_PlayerAnim.h"
#include "Nekiro/Character/K_Boss.h"
#include "Nekiro/Components/K_StatusComp.h"
#include "NEKIRO/Data/K_DataAssets.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

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
	if (!playerOwner)
	{
		playerOwner = Cast<AK_Player> ( GetOwner () );
	}

	if (bIsAttacking)
	{
		if (bCanAcceptNextComboInput)
		{
			ProceedToNextCombo ();
		}
		else
		{
			bQueuedNextComboInput = true;

			UWorld* world = GetWorld ();
			if (world)
			{
				world->GetTimerManager ().ClearTimer ( comboInputTimerHandle );
				const float inputLimitTime = combatData? combatData->COMBO_INPUT_LIMIT_TIME : 0.3f;
				world->GetTimerManager ().SetTimer ( comboInputTimerHandle , this , &UK_ActionComp::ClearQueuedCombo , inputLimitTime , false );
			}
		}
		return;
	}

	bIsAttacking = true;
	bCanAcceptNextComboInput = false;
	bQueuedNextComboInput = false;
	currentComboIndex = 0;
	hitActors.Empty ();

	if (playerOwner)
	{
		playerOwner->SetWeaponCollision( false );
	}

	

	OnAttackStateDel.Broadcast ( bIsAttacking , currentComboIndex );
}

void UK_ActionComp::HandleNextAttackCheck ()
{
	//UE_LOG ( LogTemp , Warning , TEXT ( "HandleNextAttackCheck called! bIsAttacking: %d, bQueuedNextComboInput: %d" ) , bIsAttacking , bQueuedNextComboInput );

	if(!bIsAttacking)
	{
		return;
	}

	bCanAcceptNextComboInput = true;

	UWorld* world = GetWorld ();
	if (world)
	{
		world->GetTimerManager ().ClearTimer ( comboResetTimerHandle );
		const float resetDelay = combatData ? combatData->COMBO_RESET_DELAY : 0.3f;
		world->GetTimerManager ().SetTimer ( comboResetTimerHandle , this , &UK_ActionComp::ResetCombo , resetDelay , false );
	}

	if (bQueuedNextComboInput)
	{
		//UE_LOG ( LogTemp , Warning , TEXT ( "HandleNextAttackCheck - Processing queued input!" ) );
		ProceedToNextCombo ();
	}
}

void UK_ActionComp::HandleAttackHitCheck ()
{
	if (!bIsAttacking)
	{
		return;
	}

	if (!playerOwner)
	{
		playerOwner = Cast<AK_Player> ( GetOwner () );
	}

	USphereComponent* attackCollision = playerOwner->GetWeaponCollision ();
	if(!attackCollision)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Attack Collision is Null" ) );
		return;
	}

	playerOwner->SetWeaponCollision ( true );
	attackCollision->UpdateOverlaps ();

	TArray<AActor*> overlappingActors;
	attackCollision->GetOverlappingActors ( overlappingActors, AK_Boss::StaticClass());

	const float damage = combatData ? combatData->DEFAULT_ATTACK_DAMAGE : 10.f;

	for(AActor* actor : overlappingActors)
	{
		AK_Boss* boss = Cast<AK_Boss> ( actor );

		if (!boss || hitActors.Contains ( boss ))
		{
			continue;
		}

		hitActors.Add ( boss );

		UGameplayStatics::ApplyDamage ( boss , damage , playerOwner->GetController () , playerOwner , nullptr );

		/*UK_StatusComp* bossStatus = boss->GetStatusComp ();
		if (bossStatus)
		{
			bossStatus->TakeDamage ( damage );
		}*/

		if (hitEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation ( GetWorld () , hitEffect ,
				attackCollision->GetComponentLocation(),
				attackCollision->GetComponentRotation());
		}
	}

	playerOwner->SetWeaponCollision ( false );
}

void UK_ActionComp::HandleAttackFinished ()
{
	if (!bIsAttacking)
	{
		return;
	}

	bIsAttacking = false;
	bCanAcceptNextComboInput = false;
	bQueuedNextComboInput = false;
	currentComboIndex = 0;
	hitActors.Empty ();

	if (playerOwner)
	{
		playerOwner->SetWeaponCollision ( false );
	}

	UWorld* world = GetWorld ();
	if (world)
	{
		world->GetTimerManager ().ClearTimer ( comboInputTimerHandle );
		world->GetTimerManager ().ClearTimer ( comboResetTimerHandle );
	}

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
	TArray<AActor*> ignoreActors;
	ignoreActors.Add ( player );

	FHitResult outHit;
	/*bool bIsHit = UKismetSystemLibrary::SphereTraceSingleForObjects ( GetWorld () , 
		start , end , 30.f , 
		objectTypes , false , ignoreActors , 
		EDrawDebugTrace::ForDuration , outHit , true,
		FLinearColor::Red, FLinearColor::Green, 2.0f);*/

	bool bIsHit = UKismetSystemLibrary::SphereTraceSingleForObjects ( GetWorld () ,
		start , end , 30.f ,
		objectTypes , false , ignoreActors ,
		EDrawDebugTrace::None , outHit , true );

	if (bIsHit)
	{
		AK_Boss* lockOnTarget = Cast<AK_Boss> ( outHit.GetActor () );
		if (lockOnTarget)
		{
			targetBoss = lockOnTarget;
			bIsLockOn = true;
			OnLockOnStateDel.Broadcast ( true , targetBoss );

			UE_LOG ( LogTemp , Warning , TEXT ( "Lock-On Target : %s" ) , *targetBoss->GetName () );
			return;
		}
		else
		{
			//UE_LOG ( LogTemp , Warning , TEXT ( "Lock-On Target is not Boss Type" ) );
		}
	}
	else
	{
		//UE_LOG ( LogTemp , Warning , TEXT ( "No Lock-On Hit Detected" ) );
	}

	//락온 실패
	{
		bIsLockOn = false;
		OnLockOnStateDel.Broadcast ( false , nullptr );

		//UE_LOG ( LogTemp , Warning , TEXT ( "Lock-On Failed" ) );
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
	OnLockOnStateDel.Broadcast ( false , nullptr );

	//UE_LOG ( LogTemp , Warning , TEXT ( "Lock-On Released" ) );
}

void UK_ActionComp::ResetCombo ()
{
	HandleAttackFinished ();
}

void UK_ActionComp::ProceedToNextCombo ()
{
	//UE_LOG ( LogTemp , Warning , TEXT ( "ProceedToNextCombo is Called!!!" ) );

	if(!bIsAttacking)
	{
		return;
	}

	const int32 maxComboCount = attackSectionNames.Num () > 0 ? attackSectionNames.Num () : currentComboIndex + 1;

	if(currentComboIndex  >= maxComboCount - 1) //safety check for last combo
	{
		//UE_LOG ( LogTemp , Warning , TEXT ( "ProceedToNextCombo - Last combo reached, index: %d" ) , currentComboIndex );

		bCanAcceptNextComboInput = false;
		bQueuedNextComboInput = false;
		return;
	}

	++currentComboIndex;
	bCanAcceptNextComboInput = false;
	bQueuedNextComboInput = false;
	hitActors.Empty ();

	UWorld* world = GetWorld ();
	if(world)
	{
		world->GetTimerManager ().ClearTimer ( comboInputTimerHandle );
		world->GetTimerManager ().ClearTimer ( comboResetTimerHandle );
	}

	UE_LOG ( LogTemp , Warning , TEXT ( "Proceeding to Combo Index : %d" ) , currentComboIndex );

	OnAttackStateDel.Broadcast ( bIsAttacking , currentComboIndex );
}

void UK_ActionComp::ClearQueuedCombo ()
{
	bQueuedNextComboInput = false;
}
