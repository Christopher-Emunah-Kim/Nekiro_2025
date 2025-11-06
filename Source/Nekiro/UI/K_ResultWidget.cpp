// Fill out your copyright notice in the Description page of Project Settings.


#include "K_ResultWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"

void UK_ResultWidget::NativeConstruct ()
{
	if (VictoryBorder)
	{
		VictoryBorder->SetVisibility ( ESlateVisibility::Hidden );
	}

	if (DefeatBorder)
	{
		DefeatBorder->SetVisibility ( ESlateVisibility::Hidden );
	}

	if (ExitButton)
	{
		ExitButton->SetVisibility ( ESlateVisibility::Hidden );
	}

	if (VictoryAnim)
	{
		FWidgetAnimationDynamicEvent animFinishedDel;
		animFinishedDel.BindDynamic ( this , &UK_ResultWidget::HandleAnimFinished );
		BindToAnimationFinished ( VictoryAnim , animFinishedDel );
	}

	if (DefeatAnim)
	{
		FWidgetAnimationDynamicEvent animFinishedDel;
		animFinishedDel.BindDynamic ( this , &UK_ResultWidget::HandleAnimFinished );
		BindToAnimationFinished ( DefeatAnim , animFinishedDel );
	}

}

void UK_ResultWidget::ShowResult ( EResultState NewResultState )
{
	currentResultState = NewResultState;

	switch (currentResultState)
	{
	case EResultState::Victory:
	{
		if (VictoryBorder)
		{
			VictoryBorder->SetVisibility ( ESlateVisibility::Visible );
		}
		if (VictoryAnim)
		{
			PlayAnimation ( VictoryAnim );
		}
	}
		break;
	case EResultState::Defeat:
	{
		if (DefeatBorder)
		{
			DefeatBorder->SetVisibility ( ESlateVisibility::Visible );
		}
		if (DefeatAnim)
		{
			PlayAnimation ( DefeatAnim );
		}
	}
		break;
	default:
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Unknown Result State!!!!!!!!!!!!" ) );
	}
		break;
	}
}

void UK_ResultWidget::HandleAnimFinished ()
{
	if(ExitButton)
	{
		ExitButton->SetVisibility ( ESlateVisibility::Visible );
	}

	if (QuitButtonAnim)
	{
		PlayAnimation ( QuitButtonAnim );
	}
}
