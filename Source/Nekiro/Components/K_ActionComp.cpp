// Fill out your copyright notice in the Description page of Project Settings.

#include "K_ActionComp.h"

// Sets default values for this component's properties
UK_ActionComp::UK_ActionComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UK_ActionComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UK_ActionComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

