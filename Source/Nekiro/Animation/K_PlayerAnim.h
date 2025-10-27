// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "K_PlayerAnimStates.h"

#include "K_PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class NEKIRO_API UK_PlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

	UK_PlayerAnim () = default;

public:
	virtual void NativeInitializeAnimation () override;
	virtual void NativeUpdateAnimation ( float DeltaSeconds ) override;

	void SetPlayerCharacter ( class AK_Player& player );

protected:
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	FPlayerAnimStates m_playerAnimStates;

	UPROPERTY(Transient)
	TObjectPtr<class AK_Player> m_player;
	

};
