// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class NEKIRO_API UBossHPWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeTick ( const FGeometry& MyGeometry , float InDeltaTime ) override;

	void SetHPPercent ( float curHP, float maxHP );
	
protected:
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HPBar;

};
