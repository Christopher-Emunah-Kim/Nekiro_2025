// Fill out your copyright notice in the Description page of Project Settings.
#include "K_Player.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>

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

	cameraComp = CreateDefaultSubobject<UCameraComponent> ( TEXT ( "CameraComp" ) );
	cameraComp->SetupAttachment ( springArmComp);
	cameraComp->SetRelativeLocation ( FVector ( 0.f , 0.f , 90.f ) );

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
	UpdateAnimState ( 0.f );
}

// Called every frame
void AK_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateAnimState ( DeltaTime );
}

// Called to bind functionality to input
void AK_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* inputComp = Cast<UEnhancedInputComponent> ( PlayerInputComponent );
	if (inputComp)
	{
		inputComp->BindAction ( IA_Move , ETriggerEvent::Triggered , this , &AK_Player::PlayerMove );
		inputComp->BindAction ( IA_Dash , ETriggerEvent::Started , this , &AK_Player::PlayerDash );
		inputComp->BindAction ( IA_Look , ETriggerEvent::Triggered , this , &AK_Player::PlayerLook );
		inputComp->BindAction ( IA_Jump , ETriggerEvent::Started , this , &AK_Player::PlayerJump );
		inputComp->BindAction ( IA_Crouch , ETriggerEvent::Started , this , &AK_Player::PlayerCrouch );
		inputComp->BindAction ( IA_Function , ETriggerEvent::Started , this , &AK_Player::PlayerInteraction );
		inputComp->BindAction ( IA_Finish , ETriggerEvent::Started , this , &AK_Player::PlayerFinish );
		inputComp->BindAction ( IA_Defense , ETriggerEvent::Started , this , &AK_Player::PlayerDefense );
		inputComp->BindAction ( IA_Attack , ETriggerEvent::Started , this , &AK_Player::PlayerAttack );
	}
}

void AK_Player::PlayerMove ( const FInputActionValue& value )
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

void AK_Player::PlayerLook ( const FInputActionValue& value )
{
	FVector2D lookAxisVector = value.Get<FVector2D> ();

	AK_PlayerController* pc = Cast<AK_PlayerController> ( GetController () );
	if (pc)
	{
		AddControllerYawInput ( lookAxisVector.X * mouseSensitivity );
		AddControllerPitchInput ( -lookAxisVector.Y * mouseSensitivity );
	}
}

void AK_Player::PlayerJump ()
{
	Jump ();
}

void AK_Player::PlayerDash ( const FInputActionValue& value )
{
}

void AK_Player::PlayerCrouch ( const FInputActionValue& value )
{
}

void AK_Player::PlayerAttack ()
{
}

void AK_Player::PlayerInteraction ()
{
}

void AK_Player::PlayerFinish ()
{
}

void AK_Player::PlayerDefense ()
{
}

void AK_Player::UpdateAnimState ( float deltaTime )
{
	const FVector velocity = GetVelocity ();
	const float speed = velocity.Size ();
	playerAnimStates.speed = speed;

	if(speed > 0)
	{
		const FRotator rot = GetActorRotation ();
		const FVector localVelocity = rot.UnrotateVector ( velocity );
		playerAnimStates.direction = FMath::Atan2 ( localVelocity.Y , localVelocity.X ) * (180.f / PI);
	}
	else
	{
		playerAnimStates.direction = 0.f;
	}

	const UCharacterMovementComponent* moveComp = GetCharacterMovement ();
	bool isInAir = false;
	if(moveComp)
	{
		isInAir = moveComp->IsFalling ();
	}
	else
	{
		isInAir = false;
	}
	playerAnimStates.bIsInAir = isInAir;

	if(isInAir)
	{
		playerAnimStates.movementState = EPlayerMovementState::JUMP;
	}
	else if(speed > 200.0f)
	{
		playerAnimStates.movementState = EPlayerMovementState::RUN;
	}
	else if(speed > 0.0f)
	{
		playerAnimStates.movementState = EPlayerMovementState::WALK;
	}
	else
	{
		playerAnimStates.movementState = EPlayerMovementState::IDLE;
	}

	const bool isAttacking = actionComp && actionComp->IsAttacking ();
	playerAnimStates.bIsAttack = isAttacking;
	if(isAttacking)
	{
		playerAnimStates.combatState = EPlayerCombatState::Attack;
	}
	else
	{
		playerAnimStates.combatState = EPlayerCombatState::None;
	}
}

