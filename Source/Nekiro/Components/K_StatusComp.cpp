// Fill out your copyright notice in the Description page of Project Settings.


#include "K_StatusComp.h"
#include "../Character/K_Player.h"
#include "../Character/K_Boss.h"
#include "../Animation/K_PlayerAnim.h"
#include "../Data/K_DataAssets.h"

// Sets default values for this component's properties
UK_StatusComp::UK_StatusComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UK_StatusComp::BeginPlay()
{
	Super::BeginPlay();


	bIsPlayerOwner = GetOwner() && GetOwner ()->IsA<AK_Player> ();

	if(bIsPlayerOwner)
	{
		currentHealth = statusData? statusData->PLAYER_MAX_HEALTH : 100.0f;
	}
	else 
	{
		currentHealth = statusData? statusData->BOSS_MAX_HEALTH : 500.0f;
	}
}

void UK_StatusComp::InitializeComponent ()
{
	Super::InitializeComponent ();
	// ...
}

void UK_StatusComp::TakeDamage(float damageAmount)
{
	if (damageAmount <= 0.f)
	{
		return;
	}

	if (!statusData)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "StatusData is not assigned in StatusComp of %s" ) , *GetOwner ()->GetName () );
		return;
	}

	currentHealth = FMath::Max ( currentHealth - damageAmount , 0.f );
	UE_LOG ( LogTemp , Warning , TEXT ( "UK_StatusComp - %s 가 %f 데미지를 입었습니다. 현재 체력 : %f" ) , 
		GetOwner () ? *GetOwner ()->GetName () : TEXT ( "Unknown" ) , damageAmount , currentHealth );

	if (FMath::IsNearlyZero(currentHealth))
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "UK_StatusComp - %s 사망 처리 필요" ) , GetOwner () ? *GetOwner ()->GetName () : TEXT ( "Unknown" ) );

		if(bIsPlayerOwner)
		{
			AK_Player* playerOwner = Cast<AK_Player> ( GetOwner () );
			if (playerOwner)
			{
				//사망 델리게이트 처리
			}
		}
		else
		{
			AK_Boss* bossOwner = Cast<AK_Boss> ( GetOwner () );
			if (bossOwner)
			{
				//사망 델리게이트 처리
			}
		}
	}

}


// Called every frame
void UK_StatusComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

