// Fill out your copyright notice in the Description page of Project Settings.
#include "K_Player.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Blueprint/UserWidget.h>

#include "K_PlayerController.h"
#include "Nekiro/Animation/K_PlayerAnim.h"
#include "Nekiro/Data/K_DataAssets.h"
#include "Nekiro/Components/K_StatusComp.h"
#include "Nekiro/Components/K_ActionComp.h"
#include "Nekiro/UI/K_MainHUDUI.h"
#include "Nekiro/UI/K_ResultWidget.h"
#include "K_Boss.h"

// Sets default values
AK_Player::AK_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitializeComponents ();

	static ConstructorHelpers::FClassFinder<UAnimInstance> tempABP ( TEXT ( "/Script/Engine.AnimBlueprint'/Game/Blueprints/Player/ABP_Player.ABP_Player_C'" ) );
	if (tempABP.Succeeded ())
	{
		GetMesh ()->SetAnimInstanceClass ( tempABP.Class );
	}

}

void AK_Player::InitializeComponents ()
{
	statusComp = CreateDefaultSubobject<UK_StatusComp> ( TEXT ( "StatusComp" ) );
	actionComp = CreateDefaultSubobject<UK_ActionComp> ( TEXT ( "ActionComp" ) );

	springArmComp = CreateDefaultSubobject<USpringArmComponent> ( TEXT ( "SpringArmComp" ) );
	springArmComp->SetupAttachment ( GetRootComponent () );
	springArmComp->SetRelativeLocation ( FVector ( 0.f , 0.f , 70.f ) );
	springArmComp->TargetArmLength = 300.f;
	springArmComp->bUsePawnControlRotation = true;

	cameraComp = CreateDefaultSubobject<UCameraComponent> ( TEXT ( "CameraComp" ) );
	cameraComp->SetupAttachment ( springArmComp );
	cameraComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;
	GetCharacterMovement ()->bOrientRotationToMovement = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh ( TEXT ( "/Script/Engine.SkeletalMesh'/Game/Assets/Character/ParagonKwang/Characters/Heroes/Kwang/Skins/Tier2/Kwang_Manban/Meshes/KwangManbun.KwangManbun'" ) );
	if (tempMesh.Succeeded ())
	{
		GetMesh ()->SetSkeletalMesh ( tempMesh.Object );
		GetMesh ()->SetRelativeLocation ( FVector ( 0.f , 0.f , -90.f ) );
		GetMesh ()->SetRelativeRotation ( FRotator ( 0.f , -90.f , 0.f ) );
		GetMesh ()->SetWorldScale3D ( FVector ( 0.9f , 0.9f , 0.9f ) );
	}

	//Weapon Collision Setup
	//TODO 에디터에서 소켓이름 지정
	weaponCollisionComp = CreateDefaultSubobject<USphereComponent> ( TEXT ( "WeaponCollisionComp" ) );
	weaponCollisionComp->SetupAttachment ( GetMesh () , weaponSocketName );
	weaponCollisionComp->SetSphereRadius ( 50.f );

}

// Called when the game starts or when spawned
void AK_Player::BeginPlay ()
{
	Super::BeginPlay ();

	InitializeInputSettings ();

	//Animation Instance Setup
	playerAnim = Cast<UK_PlayerAnim> ( GetMesh ()->GetAnimInstance () );
	if (!playerAnim)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "No Player Anim Instance found!" ) );
		return;
	}
	playerAnim->SetPlayerCharacter ( *this );

	//Weapon Collision Setup
	SetWeaponCollision ( false );

	//Bind Action Component Delegates
	if (actionComp)
	{
		actionComp->OnGuardStateDel.AddDynamic ( this , &AK_Player::OnGuardStateChanged );
		actionComp->OnAttackStateDel.AddDynamic ( this , &AK_Player::OnAttackStateChanged );
		actionComp->OnLockOnStateDel.AddDynamic ( this , &AK_Player::OnLockOnStateChanged );
	}

	//Bind Status Component Delegates
	if (statusComp)
	{
		statusComp->OnDeathDel.AddDynamic ( this , &AK_Player::OnPlayerDeath );
	}

	//UI Setup
	if (playerUIFactory)
	{
		APlayerController* PC = Cast<APlayerController> ( GetController () );
		if (PC)
		{
			playerHUDUI = CreateWidget<UUserWidget> ( PC , playerUIFactory );
			if (playerHUDUI)
			{
				playerHUDUI->AddToViewport ();
				UE_LOG ( LogTemp , Log , TEXT ( "Player HUD UI created successfully" ) );
			}
			else
			{
				UE_LOG ( LogTemp , Error , TEXT ( "Failed to create Player HUD UI" ) );
			}
		}
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "playerUIFactory is not set in Blueprint!" ) );
	}
}

void AK_Player::PossessedBy ( AController* newController )
{
	Super::PossessedBy ( newController );

	//InitializeInputSettings ();
}

void AK_Player::InitializeInputSettings ()
{
	//Input Mapping Context Setup
	AK_PlayerController* pc = Cast<AK_PlayerController> ( GetController () );
	if (!pc)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "No Player Controller found!" ) );
		return;
	}
	//FInputModeGameAndUI inputMode;
	//inputMode.SetLockMouseToViewportBehavior ( EMouseLockMode::LockAlways );
	//inputMode.SetHideCursorDuringCapture ( true );
	FInputModeGameOnly inputMode2;

	pc->SetInputMode ( inputMode2 );
	pc->bShowMouseCursor = false;
	pc->bEnableMouseOverEvents = false;
	

	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem> ( pc->GetLocalPlayer () );
	if (!subsys)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "No Enhanced Input Subsystem found!" ) );
		return;
	}

	subsys->AddMappingContext ( IMC_Player , 0 );
}

// Called every frame
void AK_Player::Tick ( float DeltaTime )
{
	Super::Tick ( DeltaTime );

	if (playerHUDUI)
	{
		UK_MainHUDUI* mainHUD = Cast<UK_MainHUDUI> ( playerHUDUI );
		if(mainHUD && statusComp)
		{
			mainHUD->SetHPPercent ( statusComp->GetCurrentHealth () , statusComp->GetMaxHealth () );
		}
	}
}

// Called to bind functionality to input
void AK_Player::SetupPlayerInputComponent ( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent ( PlayerInputComponent );

	UEnhancedInputComponent* inputComp = Cast<UEnhancedInputComponent> ( PlayerInputComponent );
	if (inputComp)
	{
		inputComp->BindAction ( IA_Move , ETriggerEvent::Triggered , this , &AK_Player::OnPlayerMove );
		inputComp->BindAction ( IA_Dash , ETriggerEvent::Started , this , &AK_Player::OnPlayerDash );
		inputComp->BindAction ( IA_Look , ETriggerEvent::Triggered , this , &AK_Player::OnPlayerLook );
		inputComp->BindAction ( IA_Jump , ETriggerEvent::Started , this , &AK_Player::OnPlayerJump );
		inputComp->BindAction ( IA_Crouch , ETriggerEvent::Started , this , &AK_Player::OnPlayerCrouchStarted );
		inputComp->BindAction ( IA_Crouch , ETriggerEvent::Completed , this , &AK_Player::OnPlayerCrouchCompleted );
		inputComp->BindAction ( IA_Function , ETriggerEvent::Started , this , &AK_Player::OnPlayerInteraction );
		inputComp->BindAction ( IA_Finish , ETriggerEvent::Started , this , &AK_Player::OnPlayerFinish );
		inputComp->BindAction ( IA_Defense , ETriggerEvent::Started , this , &AK_Player::OnPlayerGuardStarted );
		inputComp->BindAction ( IA_Defense , ETriggerEvent::Completed , this , &AK_Player::OnPlayerGuardCompleted );
		inputComp->BindAction ( IA_Attack , ETriggerEvent::Started , this , &AK_Player::OnPlayerAttack );
		inputComp->BindAction ( IA_LockOn , ETriggerEvent::Started , this , &AK_Player::OnPlayerLockOnOff );
	}
}

float AK_Player::TakeDamage ( float DamageAmount , FDamageEvent const& DamageEvent , AController* EventInstigator , AActor* DamageCauser )
{
	if (bIsPlayerDead)
	{
		return 0.f;
	}

	Super::TakeDamage ( DamageAmount , DamageEvent , EventInstigator , DamageCauser );

	if (!statusComp)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "No Status Component found!" ) );
		return 0.f;
	}

	return statusComp->TakeDamage ( DamageAmount );
}

void AK_Player::OnPlayerMove ( const FInputActionValue& value )
{
	FVector2D inputVector = value.Get<FVector2D> ();

	AK_PlayerController* pc = Cast<AK_PlayerController> ( GetController () );
	if (!pc)
	{
		return;
	}

	const FRotator controlRot = pc->GetControlRotation ();
	const FRotator yawRot ( 0.f , controlRot.Yaw , 0.f );

	const FVector forwardDir = FRotationMatrix ( yawRot ).GetUnitAxis ( EAxis::X );
	const FVector rightDir = FRotationMatrix ( yawRot ).GetUnitAxis ( EAxis::Y );

	AddMovementInput ( forwardDir , inputVector.Y );
	AddMovementInput ( rightDir , inputVector.X );
}

void AK_Player::OnPlayerLook ( const FInputActionValue& value )
{
	FVector2D inputVector = value.Get<FVector2D> ();

	AK_PlayerController* pc = Cast<AK_PlayerController> ( GetController () );
	if (pc)
	{
		AddControllerYawInput ( inputVector.X * mouseSensitivity );
		AddControllerPitchInput ( -inputVector.Y * mouseSensitivity );
		//UE_LOG ( LogTemp , Warning , TEXT ( "Player Look Input X: %f , Y: %f" ) , inputVector.X , inputVector.Y );
	}
}

void AK_Player::OnPlayerJump ()
{
	Jump ();
}

void AK_Player::OnPlayerDash ( const FInputActionValue& value )
{
	FVector dashDirection = GetLastMovementInputVector ();
	if(dashDirection.IsNearlyZero ())
	{
		dashDirection = GetActorForwardVector ();
	}

	float dashStrength = movementData->DASH_STRENGTH;

	LaunchCharacter ( dashDirection.GetSafeNormal () * dashStrength , true , true );

	if (playerAnim && !playerAnim->GetIsCrouch ())
	{
		playerAnim->PlayDashMontage ();
	}
}

void AK_Player::OnPlayerCrouchStarted ( const FInputActionValue& value )
{
	//Crouch ();
	GetCharacterMovement ()->MaxWalkSpeed = movementData->CROUCH_SPEED;
	
	if (playerAnim)
	{
		playerAnim->SetIsCrouch ( true );
	}
}

void AK_Player::OnPlayerCrouchCompleted ( const FInputActionValue& value )
{
	//UnCrouch ();
	GetCharacterMovement ()->MaxWalkSpeed = movementData->SPRINT_SPEED;

	if (playerAnim)
	{
		playerAnim->SetIsCrouch ( false );
	}
}

void AK_Player::OnPlayerLockOnOff ( const FInputActionValue& value )
{
	if(!actionComp)
	{
		return;
	}

	if(actionComp->IsLockOn())
	{
		actionComp->CompleteLockOn ();
	}
	else
	{
		actionComp->StartLockOn ();
	}
}


void AK_Player::OnPlayerAttack ( const FInputActionValue& value )
{
	if(actionComp)
	{
		actionComp->PerformAttack ();
	}	
}

void AK_Player::OnPlayerInteraction ()
{
}

void AK_Player::OnPlayerFinish ()
{
}

void AK_Player::SetWeaponCollision ( bool bEnable )
{
	if (!weaponCollisionComp)
	{
		return;
	}

	if (bEnable)
	{
		weaponCollisionComp->SetCollisionEnabled ( ECollisionEnabled::QueryAndPhysics );
	}
	else
	{
		weaponCollisionComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	}

	weaponCollisionComp->SetGenerateOverlapEvents ( bEnable );
}

void AK_Player::OnPlayerGuardStarted ( const FInputActionValue& value )
{
	if (playerAnim->GetIsCrouch ())
	{
		return;
	}

	if (actionComp)
	{
		actionComp->StartGuard ();
	}
}

void AK_Player::OnPlayerGuardCompleted ( const FInputActionValue& value )
{
	if(actionComp)
	{
		actionComp->CompleteGuard ();
	}
}

void AK_Player::OnGuardStateChanged ( bool bIsGuarding )
{
	if (playerAnim)
	{
		if(bIsGuarding)
		{
			playerAnim->EnterGuardState();
		}
		else
		{
			playerAnim->ExitGuardState ();
		}
	}

	if(movementData)
	{
		if(bIsGuarding)
		{
			GetCharacterMovement ()->MaxWalkSpeed = movementData->CROUCH_SPEED;
		}
		else
		{
			GetCharacterMovement ()->MaxWalkSpeed = movementData->SPRINT_SPEED;
		}
	}

}

void AK_Player::OnAttackStateChanged ( bool bIsAttacking , int32 ComboIndex )
{
	if (!playerAnim)
	{
		return;
	}

	if (bIsAttacking)
	{
		playerAnim->EnterAttatkState(ComboIndex);
	}
	else
	{
		playerAnim->ExitAttackState ();
	}
}

void AK_Player::OnLockOnStateChanged ( bool bIsLockOnParam , AK_Boss* targetBoss )
{
	if (bIsLockOnParam)
	{
		//bUseControllerRotationYaw = false;
		GetCharacterMovement ()->bOrientRotationToMovement = true;

		UE_LOG ( LogTemp , Warning , TEXT ( "Player Lock-On Enabled" ) );
	}
	else
	{
		//카메라 세팅 복구
		bUseControllerRotationYaw = true;
		GetCharacterMovement ()->bOrientRotationToMovement = false;

		UE_LOG ( LogTemp , Warning , TEXT ( "Player Lock-On Disabled" ) );
	}

	UpdateLockOnWBP ( bIsLockOnParam , targetBoss );
}

void AK_Player::UpdateLockOnWBP_Implementation ( bool bIsLockOn , AK_Boss* targetBoss )
{
	//To be implemented in Blueprint
}



void AK_Player::OnPlayerDeath ()
{
	UE_LOG ( LogTemp , Warning , TEXT ( "Player Death Function Called" ) );

	if(bIsPlayerDead)
	{
		return;
	}
	bIsPlayerDead = true;

	DisableInput ( Cast<APlayerController> ( GetController () ) );
	GetCapsuleComponent()->SetCollisionEnabled (ECollisionEnabled::NoCollision);
	GetCharacterMovement ()->DisableMovement ();

	if (playerAnim)
	{
		playerAnim->PlayDeathMontage ();
		playerAnim->SetIsDead ( true );
	}

}

void AK_Player::ShowResultUI( bool bIsPlayerVictory )
{
	APlayerController* pc = Cast<APlayerController> ( GetController () );

	if (!pc || !resultUIFactory)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "No Player Controller or Result UI Factory found!" ) );
		return;
	}

	if(!resultUI)
	{
		resultUI = CreateWidget<UUserWidget> ( pc , resultUIFactory );
	}

	if(!resultUI)
	{
		UE_LOG ( LogTemp , Error , TEXT ( "Failed to create Result UI" ) );
		return;
	}

	resultUI->AddToViewport (10);
	UE_LOG ( LogTemp , Log , TEXT ( "Result UI created successfully" ) );

	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus ( resultUI->TakeWidget () );
	inputMode.SetLockMouseToViewportBehavior ( EMouseLockMode::DoNotLock );
	pc->SetInputMode ( inputMode );
	pc->bShowMouseCursor = true;

	UGameplayStatics::SetGamePaused ( GetWorld () , true );
	
	UK_ResultWidget* result = Cast<UK_ResultWidget> ( resultUI );
	if(result)
	{
		if (bIsPlayerVictory)
		{
			result->ShowResult( EResultState::Victory );
		}
		else
		{
			result->ShowResult( EResultState::Defeat );
		}
	}
}