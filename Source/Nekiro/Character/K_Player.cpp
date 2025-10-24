// Fill out your copyright notice in the Description page of Project Settings.
#include "K_Player.h"

// Sets default values
AK_Player::AK_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatusComp = CreateDefaultSubobject<UK_StatusComp> ( TEXT ( "StatusComp" ) );
	ActionComp = CreateDefaultSubobject<UK_ActionComp> ( TEXT ( "ActionComp" ) );

}

// Called when the game starts or when spawned
void AK_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AK_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AK_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

