// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHPWidget.h"
#include "Components/ProgressBar.h"

void UBossHPWidget::NativeTick ( const FGeometry& MyGeometry , float InDeltaTime )
{
	Super::NativeTick ( MyGeometry , InDeltaTime );
}

void UBossHPWidget::SetHPPercent ( float curHP, float maxHP )
{
	if ( HPBar )
	{
		HPBar->SetPercent ( curHP / maxHP );
	}
}
