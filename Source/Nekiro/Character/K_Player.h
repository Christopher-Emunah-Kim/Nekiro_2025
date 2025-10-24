// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Nekiro/Components/K_StatusComp.h"
#include "Nekiro/Components/K_ActionComp.h"

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

	UFUNCTION ( BlueprintPure , Category = "Sekiro|Components" )
	UK_StatusComp* GetStatusComp () const { return StatusComp; }

	UFUNCTION ( BlueprintPure , Category = "Sekiro|Components" )
	UK_ActionComp* GetActionComp () const { return ActionComp; }


private:
	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "Sekiro|Components" , meta = (AllowPrivateAccess = "true") )
	class UK_StatusComp* StatusComp;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "Sekiro|Components" , meta = (AllowPrivateAccess = "true") )
	class UK_ActionComp* ActionComp;

};
