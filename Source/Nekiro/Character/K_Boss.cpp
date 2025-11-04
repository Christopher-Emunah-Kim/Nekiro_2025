// Fill out your copyright notice in the Description page of Project Settings.


#include "K_Boss.h"
#include "K_BossAIController.h"
#include "NEKIRO/Character/K_Player.h"
#include "NEKIRO/Animation/K_BossAnim.h"
#include "NEKIRO/Components/K_ActionComp.h"
#include "NEKIRO/Components/K_StatusComp.h"
#include "NEKIRO/Data/K_DataAssets.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/GameplayStatics.h>

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
		bossMovementComp->RotationRate = FRotator ( 0.f , 360.f , 0.f );
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh ( TEXT ( "/Script/Engine.SkeletalMesh'/Game/Assets/Character/Mixamo/X_Bot.X_Bot'" ) );
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


	katanaMeshComp = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "KatanaMeshComp" ) );
	katanaMeshComp->SetupAttachment ( GetMesh () , TEXT ( "RightHandSocketSheath" ) );
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempKatanaMesh ( TEXT ( "/Game/Assets/Character/GhostSamurai_Bundle/GhostSamurai/Weapon/Mesh/Katana/SM_Katana01.SM_Katana01" ) );
	if (tempKatanaMesh.Succeeded ())
	{
		katanaMeshComp->SetStaticMesh ( tempKatanaMesh.Object );
		katanaMeshComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
		katanaMeshComp->SetCollisionObjectType ( ECC_WorldDynamic );
		katanaMeshComp->SetCollisionResponseToAllChannels ( ECR_Ignore );
		katanaMeshComp->SetCollisionResponseToChannel ( ECC_Pawn , ECR_Overlap );
		katanaMeshComp->SetGenerateOverlapEvents ( false );
	}


	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AK_BossAIController::StaticClass ();

	statusComp = CreateDefaultSubobject<UK_StatusComp> ( TEXT ( "StatusComp" ) );
	
	bossAnimStates.bIsAttack = false;
	bossAnimStates.actionName = NAME_None;
}

// Called when the game starts or when spawnedㄴ
void AK_Boss::BeginPlay()
{
	Super::BeginPlay();
	
	UCharacterMovementComponent* bossMovementComp = GetCharacterMovement ();
	bossMovementComp->MaxWalkSpeed = movementData? movementData->BOSS_RUN_SPEED : 600.0f;

	if (statusComp)
	{
		statusComp->OnDeathDel.AddDynamic ( this , &AK_Boss::OnBossDeath );
	}

	BindAnimDelegateActions ();
}

void AK_Boss::OnBossStateChanged (FName actionName , bool bIsActive )
{
	bossAnimStates.bIsAttack = bIsActive;
	if (bIsActive == true)
	{
		bossAnimStates.actionName = actionName;
		//StartAction
	}
	else
	{
		bossAnimStates.actionName = NAME_None;
		//EndAction
	}
}

void AK_Boss::OnAttackEnd ()
{
	OnBossAttackEndDel.Broadcast ();
}

void AK_Boss::BindAnimDelegateActions ()
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
		bossAnim->OnBossAttackAnimEndDel.AddDynamic ( this , &AK_Boss::OnAttackEnd );
	}
}

// Called every frame
void AK_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AK_Boss::TakeDamage ( float DamageAmount , FDamageEvent const& DamageEvent , AController* EventInstigator , AActor* DamageCauser )
{
	Super::TakeDamage ( DamageAmount , DamageEvent , EventInstigator , DamageCauser );

	if (!statusComp)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "No Status Component found!" ) );
		return 0.f;
	}

	return statusComp->TakeDamage ( DamageAmount );
}

void AK_Boss::ReqeustAttack(const FName& attackName)
{
	BindAnimDelegateActions ();

	if (!bossAnim)
	{
		return;
	}

	bossAnim->PlayBossAttackMontage ( attackName );

	UE_LOG ( LogTemp , Warning , TEXT ( "Boss Request Attack: %s" ) , *attackName.ToString () );
}

float AK_Boss::GetBossAttackRange () const
{
	return movementData ? movementData->BOSS_ATTACK_RANGE : 200.0f;
}

void AK_Boss::PerformAttack ( )
{
	if(!katanaMeshComp)
	{
		return;
	}

	katanaMeshComp->SetCollisionEnabled ( ECollisionEnabled::QueryAndPhysics );
	katanaMeshComp->SetGenerateOverlapEvents ( true );
	katanaMeshComp->UpdateOverlaps ();

	TArray<AActor*> overlappingActors;
	katanaMeshComp->GetOverlappingActors ( overlappingActors, AK_Player::StaticClass() );

	float bossAttackDamage = bossCombatData ? bossCombatData->BOSS_DEFAULT_ATTACK_DAMAGE : 20.f;

	for(AActor* actor : overlappingActors)
	{
		AK_Player* player = Cast<AK_Player> ( actor );
		if (player)
		{
			UGameplayStatics::ApplyDamage ( player , bossAttackDamage , GetController () , this , UDamageType::StaticClass () );
			UE_LOG ( LogTemp , Warning , TEXT ( "Boss Attack Hit Player!" ) );
		}
	}

	katanaMeshComp->SetGenerateOverlapEvents ( false );
	katanaMeshComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
}

void AK_Boss::OnBossDeath ()
{
	UE_LOG ( LogTemp , Warning , TEXT ( "Boss Death Function Called!" ) );

	AK_BossAIController* bossAIController = Cast<AK_BossAIController> ( GetController () );
	if (bossAIController)
	{
		bossAIController->ClearTargetActor ();
		bossAIController->StopMovement ();
	}

	GetCapsuleComponent ()->SetCollisionEnabled ( ECollisionEnabled::NoCollision );

	//Anim
	if (bossAnim)
	{
		//bossAnim->PlayBossDeathMontage ();
	}

	//TODO : Death Logic (Ragdoll , Destroy Actor after delay, Rewards, UI , etc)
}
