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
	UK_StatusComp* GetStatusComp () const { return m_statusComp; }

	UFUNCTION ( BlueprintPure , Category = "NEKIRO|Components" )
	UK_ActionComp* GetActionComp () const { return m_actionComp; }



private:
	//Components


	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|Components" , meta = (AllowPrivateAccess = "true") )
	class UK_StatusComp* m_statusComp;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|Components" , meta = (AllowPrivateAccess = "true") )
	class UK_ActionComp* m_actionComp;


	//Data Assets
	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|DataAssets" , meta = (AllowPrivateAccess = "true" , ToolTip = "Player Damage Data Asset") )
	TSoftObjectPtr<UK_DamageData> m_damageDataAsset;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|DataAssets" , meta = (AllowPrivateAccess = "true" , ToolTip = "Player Combo Data Asset") )
	TSoftObjectPtr<UK_ComboData> m_comboDataAsset;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|DataAssets" , meta = (AllowPrivateAccess = "true" , ToolTip = "Player Defense Data Asset") )
	TSoftObjectPtr<UK_DefenseData> m_defenseDataAsset;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|DataAssets" , meta = (AllowPrivateAccess = "true" , ToolTip = "Player Movement Data Asset") )
	TSoftObjectPtr<UK_MovementData> m_movementDataAsset;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|DataAssets" , meta = (AllowPrivateAccess = "true" , ToolTip = "Player Posture Data Asset") )
	TSoftObjectPtr<UK_PostureData> m_postureDataAsset;

};
