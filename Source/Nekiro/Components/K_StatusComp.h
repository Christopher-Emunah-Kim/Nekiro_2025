// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "K_StatusComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE ( FOnDeathStateDelegate );

class UK_StatusData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEKIRO_API UK_StatusComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UK_StatusComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeComponent () override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY ( BlueprintAssignable , Category = "NEKIRO|StatusComp|Delegates" )
	FOnDeathStateDelegate OnDeathDel;


	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|StatusComp|Status" )
	float GetCurrentHealth () const { return currentHealth; }

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|StatusComp|Status" )
	float GetMaxHealth () const;

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|StatusComp|Status" )
	void SetCurrentHealth ( float newHealth ) { currentHealth = newHealth; }

	UK_StatusData* GetStatusData () const { return statusData; }

	UFUNCTION ( BlueprintCallable , Category = "NEKIRO|StatusComp|Status" )
	float TakeDamage ( float damageAmount );

private:

	UPROPERTY ( EditDefaultsOnly , BlueprintReadOnly , Category = "NEKIRO|StatusComp|DataAssets" , meta = (AllowPrivateAccess = "true" , ToolTip = "Player Movement Data Asset") )
	UK_StatusData* statusData;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = "NEKIRO|StatusComp|Status" , meta = (AllowPrivateAccess = "true" , ToolTip = "Current Health") )
	float currentHealth;

	bool bIsPlayerOwner;

};
