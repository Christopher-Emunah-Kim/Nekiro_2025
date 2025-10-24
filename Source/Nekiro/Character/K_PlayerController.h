// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "K_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NEKIRO_API AK_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AK_PlayerController();

	virtual void BeginPlay () override;
	
};
