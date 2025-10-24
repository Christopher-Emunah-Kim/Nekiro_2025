// Fill out your copyright notice in the Description page of Project Settings.

#include "K_PlayerController.h"
#include "EnhancedInputSubsystems.h"

AK_PlayerController::AK_PlayerController ()
{
	bShowMouseCursor = false;
}

void AK_PlayerController::BeginPlay ()
{
	Super::BeginPlay ();

	UE_LOG(LogTemp , Warning, TEXT ( "[PlayerController] BeginPlay 호출됨" ) );

	ULocalPlayer* localPlayer = GetLocalPlayer ();
	if (localPlayer)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem> ();
		if (subsys)
		{
			subsys->ClearAllMappings ();
			UE_LOG(LogTemp , Warning , TEXT("[PlayerController] Enhanced Input Subsystem 초기화" ) );
		}
	}
}
