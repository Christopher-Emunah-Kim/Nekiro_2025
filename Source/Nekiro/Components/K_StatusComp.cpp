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

float UK_StatusComp::TakeDamage(float damageAmount)
{
	if (damageAmount <= 0.f )
	{
		return 0.f;
	}

	if (!statusData)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "No Status Data found!" ) );
		return 0.f;
	}

	float newHealth = FMath::Max ( currentHealth - damageAmount , 0.f );
	SetCurrentHealth ( newHealth );

	UE_LOG ( LogTemp , Warning , TEXT ( "%s Took Damage: %f , Current Health: %f" ) , 
		bIsPlayerOwner ? TEXT ( "Player" ) : TEXT ( "Boss" ), damageAmount , newHealth );

	if (FMath::IsNearlyZero ( newHealth ))
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "%s Died!" ) , bIsPlayerOwner ? TEXT ( "Player" ) : TEXT ( "Boss" ) );
		OnDeathDel.Broadcast ();
	}

	return damageAmount;
}


// Called every frame
void UK_StatusComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UK_StatusComp::GetMaxHealth() const
{
	if (!statusData)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "StatusData is not assigned in StatusComp of %s" ) , *GetOwner ()->GetName () );
		return 100.f;
	}

	if(bIsPlayerOwner)
	{
		return statusData->PLAYER_MAX_HEALTH;
	}
	else 
	{
		return statusData->BOSS_MAX_HEALTH;
	}
}

