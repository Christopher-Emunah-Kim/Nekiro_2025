// Fill out your copyright notice in the Description page of Project Settings.
#include "K_Player.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include <Kismet/KismetMathLibrary.h>

#include "K_PlayerController.h"
#include "Nekiro/Animation/K_PlayerAnim.h"

// Sets default values
AK_Player::AK_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	statusComp = CreateDefaultSubobject<UK_StatusComp> ( TEXT ( "StatusComp" ) );
	actionComp = CreateDefaultSubobject<UK_ActionComp> ( TEXT ( "ActionComp" ) );

	springArmComp = CreateDefaultSubobject<USpringArmComponent> ( TEXT ( "SpringArmComp" ) );
	springArmComp->SetupAttachment ( GetRootComponent () );
	springArmComp->SetRelativeLocation ( FVector ( 0.f , 0.f , 70.f ) );
	springArmComp->TargetArmLength = 300.f;
	springArmComp->bUsePawnControlRotation = true;

	cameraComp = CreateDefaultSubobject<UCameraComponent> ( TEXT ( "CameraComp" ) );
	cameraComp->SetupAttachment ( springArmComp);
	cameraComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;
	GetCharacterMovement ()->bOrientRotationToMovement = false;

	meshComp = GetMesh ();
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh ( TEXT ( "/Script/Engine.SkeletalMesh'/Game/Assets/Character/ParagonKwang/Characters/Heroes/Kwang/Skins/Tier2/Kwang_Manban/Meshes/KwangManbun.KwangManbun'" ) );
	if (tempMesh.Succeeded ())
	{
		meshComp->SetSkeletalMesh ( tempMesh.Object );
		meshComp->SetRelativeLocation ( FVector ( 0.f , 0.f , -90.f ) );
		meshComp->SetRelativeRotation ( FRotator ( 0.f , -90.f , 0.f ) );
		meshComp->SetWorldScale3D ( FVector ( 0.9f , 0.9f , 0.9f ) );
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> tempABP ( TEXT ( "/Script/Engine.AnimBlueprint'/Game/Blueprints/ABP_Player.ABP_Player_C'" ) );
	if (tempABP.Succeeded ())
	{
		meshComp->SetAnimInstanceClass ( tempABP.Class );
	}
}

// Called when the game starts or when spawned
void AK_Player::BeginPlay()
{
	Super::BeginPlay();
	
	AK_PlayerController* pc = Cast<AK_PlayerController> ( GetController () );
	if (pc)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem> ( pc->GetLocalPlayer () );
		if (subsys)
		{
			subsys->AddMappingContext ( IMC_Player , 0 );
			UE_LOG ( LogTemp , Warning , TEXT ( "Input Mapping Context Added!" ) );
		}
		else
		{
			UE_LOG ( LogTemp , Warning , TEXT ( "No Enhanced Input Subsystem found!" ) );
			return;
		}
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "No Player Controller found!" ) );
		return;
	}

	playerAnim = Cast<UK_PlayerAnim>(meshComp->GetAnimInstance());
	if(nullptr == playerAnim)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "No Player Anim Instance found!" ) );
		return;
	}
	playerAnim->SetPlayerCharacter( *this );

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

	UEnhancedInputComponent* inputComp = Cast<UEnhancedInputComponent> ( PlayerInputComponent );
	if (inputComp)
	{
		inputComp->BindAction ( IA_Move , ETriggerEvent::Triggered , this , &AK_Player::OnPlayerMove );
		inputComp->BindAction ( IA_Dash , ETriggerEvent::Started , this , &AK_Player::OnPlayerDash );
		inputComp->BindAction ( IA_Look , ETriggerEvent::Triggered , this , &AK_Player::OnPlayerLook );
		inputComp->BindAction ( IA_Jump , ETriggerEvent::Started , this , &AK_Player::OnPlayerJump );
		inputComp->BindAction ( IA_Crouch , ETriggerEvent::Started , this , &AK_Player::OnPlayerCrouch );
		inputComp->BindAction ( IA_Function , ETriggerEvent::Started , this , &AK_Player::OnPlayerInteraction );
		inputComp->BindAction ( IA_Finish , ETriggerEvent::Started , this , &AK_Player::OnPlayerFinish );
		inputComp->BindAction ( IA_Defense , ETriggerEvent::Started , this , &AK_Player::OnPlayerGuardStarted );
		inputComp->BindAction ( IA_Defense , ETriggerEvent::Completed , this , &AK_Player::OnPlayerGuardCompleted );
		inputComp->BindAction ( IA_Attack , ETriggerEvent::Started , this , &AK_Player::OnPlayerAttack );
	}
}

void AK_Player::OnPlayerMove ( const FInputActionValue& value )
{
	FVector2D inputVector = value.Get<FVector2D> ();

	AK_PlayerController* pc = Cast<AK_PlayerController> ( GetController () );
	if (pc)
	{
		const FRotator controlRot = pc->GetControlRotation ();
		const FRotator yawRot ( 0.f , controlRot.Yaw , 0.f );

		const FVector forwardDir = FRotationMatrix ( yawRot ).GetUnitAxis ( EAxis::X );
		const FVector rightDir = FRotationMatrix ( yawRot ).GetUnitAxis ( EAxis::Y );

		AddMovementInput ( forwardDir , inputVector.Y );
		AddMovementInput ( rightDir , inputVector.X );
	}
}

void AK_Player::OnPlayerLook ( const FInputActionValue& value )
{
	FVector2D lookAxisVector = value.Get<FVector2D> ();

	AK_PlayerController* pc = Cast<AK_PlayerController> ( GetController () );
	if (pc)
	{
		AddControllerYawInput ( lookAxisVector.X * mouseSensitivity );
		AddControllerPitchInput ( -lookAxisVector.Y * mouseSensitivity );
	}
}

void AK_Player::OnPlayerJump ()
{
	Jump ();
}

void AK_Player::OnPlayerDash ( const FInputActionValue& value )
{
}

void AK_Player::OnPlayerCrouch ( const FInputActionValue& value )
{
	if (bIsCrouched)
	{
		UnCrouch ();
	}
	else
	{
		Crouch ();
	}

	if (playerAnim)
	{
		playerAnim->SetIsCrouch ( bIsCrouched );

		GetCharacterMovement ()->MaxWalkSpeed = movementDataAsset->crouchSpeed;
	}
}

void AK_Player::OnPlayerAttack ()
{
}

void AK_Player::OnPlayerInteraction ()
{
}

void AK_Player::OnPlayerFinish ()
{
}

void AK_Player::OnPlayerGuardStarted ()
{
	if (playerAnim)
	{
		//TODO : 이후 패링 체크 로직

		playerAnim->SetIsGuard ( true );
		UE_LOG ( LogTemp , Warning , TEXT ( "Guard Started" ) );

		GetCharacterMovement ()->MaxWalkSpeed = movementDataAsset->crouchSpeed;
		playerAnim->SetIsAttack ( false );
		playerAnim->SetCombatState ( EPlayerCombatState::Guard );
	}
}

void AK_Player::OnPlayerGuardCompleted ()
{
	if(playerAnim)
	{
		playerAnim->SetIsGuard ( false );
		UE_LOG ( LogTemp , Warning , TEXT ( "Guard Completed" ) );

		GetCharacterMovement ()->MaxWalkSpeed = movementDataAsset->sprintSpeed;
		playerAnim->SetCombatState ( EPlayerCombatState::None );
		playerAnim->SetMovementState ( EPlayerMovementState::IDLE );
	}
}


