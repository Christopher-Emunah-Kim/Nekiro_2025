// Fill out your copyright notice in the Description page of Project Settings.
#include "K_GameInstance.h"

void UK_GameInstance::Init ()
{
	Super::Init ();

	UE_LOG ( LogTemp , Log , TEXT ( "[GameInstance] 게임 인스턴스 초기화 완료" ) );
}
