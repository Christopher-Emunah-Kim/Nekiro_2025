// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "K_ResultWidget.generated.h"


UENUM ( BlueprintType )
enum class EResultState : uint8
{
	Victory,
	Defeat
};

/**
 * 
 */
UCLASS()
class NEKIRO_API UK_ResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct () override;

	UFUNCTION(BlueprintCallable, Category = "Result")
	void ShowResult(EResultState ResultState);

private:
	UFUNCTION()
	void HandleAnimFinished ();

protected:
	UPROPERTY ( Transient, meta = (BindWidgetAnim) )
	class UWidgetAnimation* VictoryAnim = nullptr;

	UPROPERTY ( Transient , meta = (BindWidgetAnim) )
	class UWidgetAnimation* DefeatAnim = nullptr;

	UPROPERTY ( Transient , meta = (BindWidgetAnim) )
	class UWidgetAnimation* QuitButtonAnim = nullptr;
	
	UPROPERTY ( Transient , meta = (BindWidget) )
	class UBorder* VictoryBorder = nullptr;

	UPROPERTY ( Transient , meta = (BindWidget) )
	class UBorder* DefeatBorder = nullptr;

	UPROPERTY( Transient , meta = (BindWidget))
	class UButton* ExitButton = nullptr;

	UPROPERTY( Transient , meta = (BindWidget))
	class UTextBlock* ExitText = nullptr;

private:
	EResultState currentResultState = EResultState::Defeat;

	
};
