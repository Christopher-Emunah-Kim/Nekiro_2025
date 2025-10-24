// Fill out your copyright notice in the Description page of Project Settings.


#include "K_StatusComp.h"

// Sets default values for this component's properties
UK_StatusComp::UK_StatusComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UK_StatusComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UK_StatusComp::InitializeComponent ()
{
	Super::InitializeComponent ();
	// ...
}


// Called every frame
void UK_StatusComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

