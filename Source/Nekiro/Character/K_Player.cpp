// Fill out your copyright notice in the Description page of Project Settings.
#include "K_Player.h"

// Sets default values
AK_Player::AK_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_statusComp = CreateDefaultSubobject<UK_StatusComp> ( TEXT ( "StatusComp" ) );
	m_actionComp = CreateDefaultSubobject<UK_ActionComp> ( TEXT ( "ActionComp" ) );

	//Data Assets
	m_damageDataAsset = TSoftObjectPtr<UK_DamageData> ( FSoftObjectPath ( TEXT ( "/Game/Blueprints/DataAssets/DA_PlayerDamage.DA_PlayerDamage'" ) ) );
	m_comboDataAsset = TSoftObjectPtr<UK_ComboData> ( FSoftObjectPath ( TEXT ( "/Game/Blueprints/DataAssets/DA_PlayerCombo.DA_PlayerCombo'" ) ) );
	m_defenseDataAsset = TSoftObjectPtr<UK_DefenseData> ( FSoftObjectPath ( TEXT ( "/Game/Blueprints/DataAssets/DA_PlayerDefense.DA_PlayerDefense'" ) ) );
	m_movementDataAsset = TSoftObjectPtr<UK_MovementData> ( FSoftObjectPath ( TEXT ( "/Game/Blueprints/DataAssets/DA_PlayerMovement.DA_PlayerMovement'" ) ) );
	m_postureDataAsset = TSoftObjectPtr<UK_PostureData> ( FSoftObjectPath ( TEXT ( "//Game/Blueprints/DataAssets/DA_PlayerPosture.DA_PlayerPosture'" ) ) );
}

// Called when the game starts or when spawned
void AK_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AK_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AK_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

