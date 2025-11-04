// Fill out your copyright notice in the Description page of Project Settings.


#include "K_MainHUDUI.h"
#include "Components/ProgressBar.h"

void UK_MainHUDUI::NativeTick ( const FGeometry& MyGeometry , float InDeltaTime )
{
	Super::NativeTick ( MyGeometry , InDeltaTime );
}

void UK_MainHUDUI::SetHPPercent ( float curHP , float maxHP )
{
	if ( HPBar )
	{
		HPBar->SetPercent ( curHP / maxHP );
	}
}