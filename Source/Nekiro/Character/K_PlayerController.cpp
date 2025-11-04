// Fill out your copyright notice in the Description page of Project Settings.

#include "K_PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "K_Player.h"

AK_PlayerController::AK_PlayerController ()
{
	bShowMouseCursor = false;
}

void AK_PlayerController::BeginPlay ()
{
	Super::BeginPlay ();

	UE_LOG(LogTemp , Warning, TEXT ( "[PlayerController] BeginPlay 호출됨" ) );

	/*ULocalPlayer* localPlayer = GetLocalPlayer ();
	if (localPlayer)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem> ();
		if (subsys)
		{
			subsys->ClearAllMappings ();
			UE_LOG(LogTemp , Warning , TEXT("[PlayerController] Enhanced Input Subsystem 초기화" ) );
		}
	}*/
}

void AK_PlayerController::OnPossess ( APawn* aPawn )
{
	Super::OnPossess ( aPawn );
	UE_LOG ( LogTemp , Warning , TEXT ( "[PlayerController] OnPossess 호출됨" ) );

	/*auto player = Cast<AK_Player> ( aPawn );
	if (player)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "[PlayerController] 플레이어 캐릭터 소유됨, 입력 설정 초기화 시작" ) );
		player->InitializeInputSettings ();
	}*/
}
