// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "Nekiro/Components/K_StatusComp.h"
#include "Nekiro/Components/K_ActionComp.h"
#include "Nekiro/Animation/K_PlayerAnimStates.h"

#include "K_Player.generated.h"

class AK_Boss;

UCLASS()
class NEKIRO_API AK_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AK_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void PerformDefaultSettings ( );

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Input Functions
	void OnPlayerMove ( const FInputActionValue& value );
	void OnPlayerLook ( const FInputActionValue& value );
	void OnPlayerDash ( const FInputActionValue& value );
	void OnPlayerCrouchStarted ( const FInputActionValue& value );
	void OnPlayerCrouchCompleted ( const FInputActionValue& value );
	void OnPlayerLockOnOff ( const FInputActionValue& value );
	void OnPlayerAttack ( const FInputActionValue& value );
	void OnPlayerGuardStarted ( const FInputActionValue& value );
	void OnPlayerGuardCompleted ( const FInputActionValue& value );
	void OnPlayerJump ();
	void OnPlayerInteraction ();
	void OnPlayerFinish ();

	//Delegate Functions
	UFUNCTION()
	void OnGuardStateChanged ( bool bIsGuarding );
	UFUNCTION()
	void OnAttackStateChanged ( bool bIsAttacking , int32 ComboIndex );

	UFUNCTION()
	void OnLockOnStateChanged ( bool bIsLockOnParam , AK_Boss* targetBoss );

	UFUNCTION ( BlueprintNativeEvent , Category = "NEKIRO|Player|UI" )
	void UpdateLockOnWBP ( bool bIsLockOn , AK_Boss* targetBoss );

	//Getters
	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Components" )
	UK_StatusComp* GetStatusComp () const { return statusComp; }

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|Components" )
	UK_ActionComp* GetActionComp () const { return actionComp; }


private:
	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|Settings" , meta = (AllowPrivateAccess = "true") )
	float mouseSensitivity = 0.3f;


	//Animation
	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|Animation" , meta = (AllowPrivateAccess = "true") )
	class UK_PlayerAnim* playerAnim;

	//Components
	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|Components" , meta = (AllowPrivateAccess = "true") )
	class USpringArmComponent* springArmComp;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|Components" , meta = (AllowPrivateAccess = "true") )
	class UCameraComponent* cameraComp;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|Components" , meta = (AllowPrivateAccess = "true") )
	class UK_StatusComp* statusComp;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|Components" , meta = (AllowPrivateAccess = "true") )
	class UK_ActionComp* actionComp;

	//IMC
	UPROPERTY ( EditDefaultsOnly, BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	class UInputMappingContext* IMC_Player;

	UPROPERTY ( EditDefaultsOnly, BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	class UInputAction* IA_Move;

	UPROPERTY ( EditDefaultsOnly, BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	class UInputAction* IA_Look;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	class UInputAction* IA_Jump;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	class UInputAction* IA_Function;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	class UInputAction* IA_Finish;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	class UInputAction* IA_Defense;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	class UInputAction* IA_Dash;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	class UInputAction* IA_Crouch;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	class UInputAction* IA_Attack;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	class UInputAction* IA_LockOn;

	//Data Assets

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|DataAssets" , meta = (AllowPrivateAccess = "true" , ToolTip = "Player Movement Data Asset") )
	class UK_MovementData* movementData;

};
