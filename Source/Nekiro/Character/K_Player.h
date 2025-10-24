// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Nekiro/Components/K_StatusComp.h"
#include "Nekiro/Components/K_ActionComp.h"
#include "Nekiro/Data/K_DataAssets.h"

#include "K_Player.generated.h"


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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;




	UFUNCTION ( BlueprintPure , Category = "NEKIRO|Components" )
	UK_StatusComp* GetStatusComp () const { return statusComp; }

	UFUNCTION ( BlueprintPure , Category = "NEKIRO|Components" )
	UK_ActionComp* GetActionComp () const { return actionComp; }



private:
	//Components

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|Components" , meta = (AllowPrivateAccess = "true") )
	class UCameraComponent* cameraComp;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|Components" , meta = (AllowPrivateAccess = "true") )
	class USkeletalMeshComponent* meshComp;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|Components" , meta = (AllowPrivateAccess = "true") )
	class UK_StatusComp* statusComp;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|Components" , meta = (AllowPrivateAccess = "true") )
	class UK_ActionComp* actionComp;

	//IMC
	UPROPERTY ( EditDefaultsOnly, BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	TSoftObjectPtr<class UInputMappingContext> IMC_Player;

	UPROPERTY ( EditDefaultsOnly, BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	TSoftObjectPtr<class UInputAction> IA_Move;

	UPROPERTY ( EditDefaultsOnly, BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	TSoftObjectPtr<class UInputAction> IA_Look;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	TSoftObjectPtr<class UInputAction> IA_Jump;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	TSoftObjectPtr<class UInputAction> IA_Function;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	TSoftObjectPtr<class UInputAction> IA_Finish;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	TSoftObjectPtr<class UInputAction> IA_Defense;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	TSoftObjectPtr<class UInputAction> IA_Dash;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	TSoftObjectPtr<class UInputAction> IA_Crouch;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|Input" , meta = (AllowPrivateAccess = "true") )
	TSoftObjectPtr<class UInputAction> IA_Attack;

	//Data Assets
	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|DataAssets" , meta = (AllowPrivateAccess = "true" , ToolTip = "Player Damage Data Asset") )
	TSoftObjectPtr<UK_DamageData> damageDataAsset;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|DataAssets" , meta = (AllowPrivateAccess = "true" , ToolTip = "Player Combo Data Asset") )
	TSoftObjectPtr<UK_ComboData> comboDataAsset;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|DataAssets" , meta = (AllowPrivateAccess = "true" , ToolTip = "Player Defense Data Asset") )
	TSoftObjectPtr<UK_DefenseData> defenseDataAsset;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|DataAssets" , meta = (AllowPrivateAccess = "true" , ToolTip = "Player Movement Data Asset") )
	TSoftObjectPtr<UK_MovementData> movementDataAsset;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|DataAssets" , meta = (AllowPrivateAccess = "true" , ToolTip = "Player Posture Data Asset") )
	TSoftObjectPtr<UK_PostureData> postureDataAsset;

};
