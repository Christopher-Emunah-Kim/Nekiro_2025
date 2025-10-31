// Fill out your copyright notice in the Description page of Project Settings.


#include "K_Boss.h"
#include "K_BossAIController.h"
#include "NEKIRO/Animation/K_BossAnim.h"
#include "NEKIRO/Components/K_ActionComp.h"
#include "NEKIRO/Components/K_StatusComp.h"
#include "NEKIRO/Data/K_DataAssets.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AK_Boss::AK_Boss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UCapsuleComponent* bossCapsuleComp = GetCapsuleComponent ();
	UCharacterMovementComponent* bossMovementComp = GetCharacterMovement ();
	if(bossCapsuleComp && bossMovementComp)
	{
		bossCapsuleComp->InitCapsuleSize ( 55.f , 96.f );
		bossCapsuleComp->SetRelativeLocation ( FVector ( 0.f , 0.f , 96.f ) );
		bossCapsuleComp->SetCollisionResponseToChannel ( ECC_Camera , ECR_Ignore );

		bossMovementComp->bUseControllerDesiredRotation = true;
		bossMovementComp->bOrientRotationToMovement = false;
		bossMovementComp->MaxWalkSpeed = 150.f;
		bossMovementComp->RotationRate = FRotator ( 0.f , 360.f , 0.f );
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh ( TEXT ( "/Script/Engine.SkeletalMesh'/Game/Assets/Character/Assassin/Mesh/SK_Assassin.SK_Assassin'" ) );
	if(tempMesh.Succeeded())
	{
		GetMesh ()->SetSkeletalMesh ( tempMesh.Object );
		GetMesh ()->SetRelativeLocation ( FVector ( 0.f , 0.f , -96.f ) );
		GetMesh ()->SetRelativeRotation ( FRotator ( 0.f , -90.f , 0.f ) );
		GetMesh ()->SetWorldScale3D ( FVector ( 1.2f , 1.2f , 1.2f ) );
		GetMesh ()->SetCollisionResponseToChannel ( ECC_Camera , ECR_Ignore );
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> tempABP ( TEXT ( "/Script/Engine.AnimBlueprint'/Game/Blueprints/Boss/ABP_Boss.ABP_Boss_C'" ) );
	if (tempABP.Succeeded ())
	{
		GetMesh ()->SetAnimInstanceClass ( tempABP.Class );
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AK_BossAIController::StaticClass ();

	actionComp = CreateDefaultSubobject<UK_ActionComp> ( TEXT ( "ActionComp" ) );
	statusComp = CreateDefaultSubobject<UK_StatusComp> ( TEXT ( "StatusComp" ) );

	bossAnimStates.bIsAttack = false;
	bossAnimStates.actionName = NAME_None;
}

// Called when the game starts or when spawned
void AK_Boss::BeginPlay()
{
	Super::BeginPlay();
	
	BindDelegateActions ();
}

void AK_Boss::OnBossStateChanged (FName actionName , bool bIsActive )
{
	bossAnimStates.bIsAttack = bIsActive;
	if (bIsActive == true)
	{
		bossAnimStates.actionName = actionName;

		if (actionComp)
		{
			//acttionComp->StartAction ( actionName );
		}
	}
	else
	{
		bossAnimStates.actionName = NAME_None;

		if (actionComp)
		{
			//actionComp->StopAction ( actionName );
		}
	}
}

void AK_Boss::BindDelegateActions ()
{
	if (bossAnim)
	{
		return;
	}

	USkeletalMeshComponent* meshComp = GetMesh ();
	if(!meshComp)
	{
		return;
	}

	bossAnim = Cast<UK_BossAnim> ( meshComp->GetAnimInstance () );
	if (bossAnim)
	{
		bossAnim->OnBossActionStateChangeDel.AddDynamic ( this , &AK_Boss::OnBossStateChanged );
	}
}

// Called every frame
void AK_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AK_Boss::ReqeustAttack(const FName& attackName)
{
	BindDelegateActions ();

	if (!bossAnim)
	{
		return;
	}


	if (actionComp)
	{
		//actionComp->RequestAction(attackName);
	}
}

void AK_Boss::ReqeustSkill(const FName& skillName)
{
	BindDelegateActions ();
	if(!bossAnim)
	{
		return;
	}


	if (actionComp)
	{
		//actionComp->RequestAction(skillName);
	}
}

