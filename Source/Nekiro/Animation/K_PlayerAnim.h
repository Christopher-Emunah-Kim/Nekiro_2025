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

private:
	void CalculateSpeedAndDirection ( float DeltaSeconds );
	void CheckPlayerStates ();
	void CheckComboAttackSections ();

public:
	virtual void NativeInitializeAnimation () override;
	virtual void NativeUpdateAnimation ( float DeltaSeconds ) override;

	void UpdateAnimStates ( float DeltaSeconds );

	void EnterGuardState ();
	void ExitGuardState ();

	void EnterAttatkState (int32 comboIndex);
	void ExitAttackState ();

	void OnAttackMontageEnded ( UAnimMontage* Montage , bool bInterrupted );

	UFUNCTION()
	void AnimNotify_AttackHitCheck ();
	
	UFUNCTION()
	void AnimNotify_NextAttackCheck ();

	void SetPlayerCharacter ( class AK_Player& player );
	void SetIsCrouch ( bool bCrouch ) { m_playerAnimStates.bIsCrouch = bCrouch; }
	bool GetIsCrouch () { return m_playerAnimStates.bIsCrouch; }

protected:
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	FPlayerAnimStates m_playerAnimStates;

	UPROPERTY(Transient)
	TObjectPtr<class AK_Player> m_player;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "NEKIRO|Animation" )
	class UAnimMontage* attackMontage;

	TArray<FName> attackSectionNames;

	float smoothSpeed = 0.f;
	FRotator smoothRot = FRotator::ZeroRotator;
};
