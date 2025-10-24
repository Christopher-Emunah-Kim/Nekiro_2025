// Fill out your copyright notice in the Description page of Project Settings.
#include "K_Player.h"

// Sets default values
AK_Player::AK_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	statusComp = CreateDefaultSubobject<UK_StatusComp> ( TEXT ( "StatusComp" ) );
	actionComp = CreateDefaultSubobject<UK_ActionComp> ( TEXT ( "ActionComp" ) );

	//Input Actions
	IMC_Player = TSoftObjectPtr<UInputMappingContext> ( FSoftObjectPath ( TEXT ( "/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_PLAYER.IMC_PLAYER'" ) ) );
	IA_Move = TSoftObjectPtr<UInputAction> ( FSoftObjectPath ( TEXT ( "/Script/EnhancedInput.InputAction'/Game/Input/IA_MOVE.IA_MOVE'" ) ) );
	IA_Look = TSoftObjectPtr<UInputAction> ( FSoftObjectPath ( TEXT ( "/Script/EnhancedInput.InputAction'/Game/Input/IA_LOOK.IA_LOOK'" ) ) );
	IA_Jump = TSoftObjectPtr<UInputAction> ( FSoftObjectPath ( TEXT ( "/Script/EnhancedInput.InputAction'/Game/Input/IA_JUMP.IA_JUMP'" ) ) );
	IA_Function = TSoftObjectPtr<UInputAction> ( FSoftObjectPath ( TEXT ( "/Script/EnhancedInput.InputAction'/Game/Input/IA_FUNCTION.IA_FUNCTION'" ) ) );
	IA_Finish = TSoftObjectPtr<UInputAction> ( FSoftObjectPath ( TEXT ( "/Script/EnhancedInput.InputAction'/Game/Input/IA_FINISH.IA_FINISH'" ) ) );
	IA_Defense = TSoftObjectPtr<UInputAction> ( FSoftObjectPath ( TEXT ( "/Script/EnhancedInput.InputAction'/Game/Input/IA_DEFENSE.IA_DEFENSE'" ) ) );
	IA_Dash = TSoftObjectPtr<UInputAction> ( FSoftObjectPath ( TEXT ( "/Script/EnhancedInput.InputAction'/Game/Input/IA_DASH.IA_DASH'" ) ) );
	IA_Crouch = TSoftObjectPtr<UInputAction> ( FSoftObjectPath ( TEXT ( "/Script/EnhancedInput.InputAction'/Game/Input/IA_CROUCH.IA_CROUCH'" ) ) );
	IA_Attack = TSoftObjectPtr<UInputAction> ( FSoftObjectPath ( TEXT ( "/Script/EnhancedInput.InputAction'/Game/Input/IA_ATTACK.IA_ATTACK'" ) ) );

	//Data Assets
	damageDataAsset = TSoftObjectPtr<UK_DamageData> ( FSoftObjectPath ( TEXT ( "/Script/Nekiro.K_DamageData'/Game/Blueprints/DataAssets/DA_PlayerDamage.DA_PlayerDamage'" ) ) );
	comboDataAsset = TSoftObjectPtr<UK_ComboData> ( FSoftObjectPath ( TEXT ( "/Script/Nekiro.K_ComboData'/Game/Blueprints/DataAssets/DA_PlayerCombo.DA_PlayerCombo'" ) ) );
	defenseDataAsset = TSoftObjectPtr<UK_DefenseData> ( FSoftObjectPath ( TEXT ( "/Script/Nekiro.K_DefenseData'/Game/Blueprints/DataAssets/DA_PlayerDefense.DA_PlayerDefense'" ) ) );
	movementDataAsset = TSoftObjectPtr<UK_MovementData> ( FSoftObjectPath ( TEXT ( "/Script/Nekiro.K_MovementData'/Game/Blueprints/DataAssets/DA_PlayerMovement.DA_PlayerMovement'" ) ) );
	postureDataAsset = TSoftObjectPtr<UK_PostureData> ( FSoftObjectPath ( TEXT ( "/Script/Nekiro.K_PostureData'/Game/Blueprints/DataAssets/DA_PlayerPosture.DA_PlayerPosture'" ) ) );
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

