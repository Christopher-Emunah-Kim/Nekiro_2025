// Fill out your copyright notice in the Description page of Project Settings.


#include "K_Boss.h"
#include "K_BossAIController.h"
#include "NEKIRO/Character/K_Player.h"
#include "NEKIRO/Animation/K_BossAnim.h"
#include "NEKIRO/Components/K_ActionComp.h"
#include "NEKIRO/Components/K_StatusComp.h"
#include "NEKIRO/Data/K_DataAssets.h"
#include "NEKIRO/UI/BossHPWidget.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AK_Boss::AK_Boss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitializeComponents ();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AK_BossAIController::StaticClass ();

	statusComp = CreateDefaultSubobject<UK_StatusComp> ( TEXT ( "StatusComp" ) );
	
	bossAnimStates.bIsAttack = false;
	bossAnimStates.actionName = NAME_None;
}

void AK_Boss::InitializeComponents ()
{
	//Capsule Component & Movement Component Setup
	UCapsuleComponent* bossCapsuleComp = GetCapsuleComponent ();
	UCharacterMovementComponent* bossMovementComp = GetCharacterMovement ();
	if (bossCapsuleComp && bossMovementComp)
	{
		bossCapsuleComp->InitCapsuleSize ( 55.f , 96.f );
		bossCapsuleComp->SetRelativeLocation ( FVector ( 0.f , 0.f , 96.f ) );
		bossCapsuleComp->SetCollisionResponseToChannel ( ECC_Camera , ECR_Ignore );

		bossMovementComp->bUseControllerDesiredRotation = true;
		bossMovementComp->bOrientRotationToMovement = false;
		bossMovementComp->RotationRate = FRotator ( 0.f , 360.f , 0.f );
	}

	//Skeletal Mesh Setup
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh ( TEXT ( "/Script/Engine.SkeletalMesh'/Game/Assets/Character/Mixamo/X_Bot.X_Bot'" ) );
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh ( TEXT ( "/Script/Engine.SkeletalMesh'/Game/Assets/Character/Assassin/Mesh/SK_Assassin.SK_Assassin'" ) );
	if (tempMesh.Succeeded ())
	{
		GetMesh ()->SetSkeletalMesh ( tempMesh.Object );
		GetMesh ()->SetRelativeLocation ( FVector ( 0.f , 0.f , -96.f ) );
		GetMesh ()->SetRelativeRotation ( FRotator ( 0.f , -90.f , 0.f ) );
		GetMesh ()->SetWorldScale3D ( FVector ( 1.2f , 1.2f , 1.2f ) );
		GetMesh ()->SetCollisionResponseToChannel ( ECC_Camera , ECR_Ignore );
	}

	//Animation Blueprint Setup
	//static ConstructorHelpers::FClassFinder<UAnimInstance> tempABP ( TEXT ( "/Script/Engine.AnimBlueprint'/Game/Blueprints/Boss/ABP_Boss.ABP_Boss_C'" ) );
	/*static ConstructorHelpers::FClassFinder<UAnimInstance> tempABP ( TEXT ( "/Script/Engine.AnimBlueprint'/Game/Blueprints/Boss/ABP_RetargetedBoss.ABP_RetargetedBoss_C'" ) );
	if (tempABP.Succeeded ())
	{
		GetMesh ()->SetAnimInstanceClass ( tempABP.Class );
	}*/

	//Katana Mesh Setup
	katanaMeshComp = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "KatanaMeshComp" ) );
	//katanaMeshComp->SetupAttachment ( GetMesh () , TEXT ( "RightHandSocketSheath" ) );
	katanaMeshComp->SetupAttachment ( GetMesh () , TEXT ( "KatanaEquippedSocket" ) );
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

	//Boss HP Widget Setup
	bossHPUIComp = CreateDefaultSubobject<UWidgetComponent> ( TEXT ( "BossHPWidget" ) );
	bossHPUIComp->SetupAttachment ( GetRootComponent () );
	ConstructorHelpers::FClassFinder<UBossHPWidget> tempHPUI ( TEXT ( "/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_BossHPUI.WBP_BossHPUI_C'" ) );
	if (tempHPUI.Succeeded ())
	{
		bossHPUIComp->SetWidgetClass ( tempHPUI.Class );
		bossHPUIComp->SetDrawSize ( FVector2D ( 150.f , 20.f ) );
		bossHPUIComp->SetRelativeLocation ( FVector ( 0.f , 0.f , 150.f ) );
	}
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

	bossHPUI = Cast<UBossHPWidget> ( bossHPUIComp->GetUserWidgetObject () );
	if (bossHPUI)
	{
		float maxHP = statusComp->GetMaxHealth ();
		bossHPUI->SetHPPercent ( maxHP , maxHP );
	}

	BindAnimDelegateActions ();
}

void AK_Boss::OnBossStateChanged (FName actionName , bool bIsActive )
{
	if(bossAnimStates.bIsDead)
	{
		return;
	}

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

	BillboardBossHPUIToCamera ();
}

void AK_Boss::BillboardBossHPUIToCamera ()
{
	FVector targetLoc = GetWorld ()->GetFirstPlayerController ()->PlayerCameraManager->GetCameraLocation ();
	FVector dir = targetLoc - bossHPUIComp->GetComponentLocation ();
	dir.Normalize ();

	FRotator rot = dir.ToOrientationRotator ();

	bossHPUIComp->SetWorldRotation ( rot );
}


float AK_Boss::TakeDamage ( float DamageAmount , FDamageEvent const& DamageEvent , AController* EventInstigator , AActor* DamageCauser )
{
	if (bossAnimStates.bIsDead)
	{
		return 0.f;
	}

	Super::TakeDamage ( DamageAmount , DamageEvent , EventInstigator , DamageCauser );

	if (!statusComp)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "No Status Component found!" ) );
		return 0.f;
	}

	float calculatedDamage = statusComp->TakeDamage ( DamageAmount );
	
	if (!bossHPUI)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "No Boss HP UI found!" ) );
		return 0.f;
	}

	float currentHP = statusComp->GetCurrentHealth ();
	float maxHP = statusComp->GetMaxHealth ();
	bossHPUI->SetHPPercent ( currentHP , maxHP );

	return calculatedDamage;
}

void AK_Boss::ReqeustAttack(const FName& attackName)
{
	BindAnimDelegateActions ();

	if (!bossAnim)
	{
		return;
	}

	if (bossAnimStates.bIsDead)
	{
		return;
	}

	currentAttackHitActors.Empty ();

	bossAnim->PlayBossAttackMontage ( attackName );

	//UE_LOG ( LogTemp , Warning , TEXT ( "Boss Request Attack: %s" ) , *attackName.ToString () );
}

float AK_Boss::GetBossAttackRange () const
{
	return movementData ? movementData->BOSS_ATTACK_RANGE : 200.0f;
}

void AK_Boss::PerformAttack ( )
{
	if (bossAnimStates.bIsDead)
	{
		return;
	}

	if(!katanaMeshComp)
	{
		return;
	}

	FVector weaponLocation = katanaMeshComp->GetComponentLocation ();
	float attackRadius = 150.0f;

	TArray<AActor*> foundActors; 
	UGameplayStatics::GetAllActorsOfClass ( GetWorld () , AK_Player::StaticClass () , foundActors );

	float bossAttackDamage = bossCombatData ? bossCombatData->BOSS_DEFAULT_ATTACK_DAMAGE : 20.f;

	for (AActor* actor : foundActors)
	{
		if (currentAttackHitActors.Contains ( actor ))
		{
			continue;
		}

		AK_Player* player = Cast<AK_Player> ( actor );
		if (!player)
		{
			continue;
		}

		float distance = FVector::Dist ( weaponLocation , player->GetActorLocation () );
		if(distance <= attackRadius)
		{
			UGameplayStatics::ApplyDamage ( player , bossAttackDamage , GetController () , this , UDamageType::StaticClass () );
			UE_LOG ( LogTemp , Warning , TEXT ( "Boss Attack Hit Player!" ) );

			currentAttackHitActors.Add ( actor );
		}
		else
		{
			UE_LOG ( LogTemp , Warning , TEXT ( "Boss Missed Player!" ) );
		}
	}

	/*katanaMeshComp->SetCollisionEnabled ( ECollisionEnabled::QueryAndPhysics );
	katanaMeshComp->SetGenerateOverlapEvents ( true );
	katanaMeshComp->UpdateOverlaps ();

	TArray<AActor*> overlappingActors;
	katanaMeshComp->GetOverlappingActors ( overlappingActors, AK_Player::StaticClass() );*/

	/*float bossAttackDamage = bossCombatData ? bossCombatData->BOSS_DEFAULT_ATTACK_DAMAGE : 20.f;

	for(AActor* actor : overlappingActors)
	{
		if(currentAttackHitActors.Contains ( actor ))
		{
			continue;
		}

		AK_Player* player = Cast<AK_Player> ( actor );
		if (player)
		{
			UGameplayStatics::ApplyDamage ( player , bossAttackDamage , GetController () , this , UDamageType::StaticClass () );
			UE_LOG ( LogTemp , Warning , TEXT ( "Boss Attack Hit Player!" ) );

			currentAttackHitActors.Add ( actor );
		}
	}*/

	/*katanaMeshComp->SetGenerateOverlapEvents ( false );
	katanaMeshComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );*/
}

void AK_Boss::OnBossDeath ()
{
	UE_LOG ( LogTemp , Warning , TEXT ( "Boss Death Function Called!" ) );

	if (bossAnimStates.bIsDead)
	{
		return;
	}

	bossAnimStates.bIsDead = true;

	AK_BossAIController* bossAIController = Cast<AK_BossAIController> ( GetController () );
	if (bossAIController)
	{
		bossAIController->ClearTargetActor ();
		bossAIController->StopMovement ();
	}

	GetCapsuleComponent ()->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	GetCharacterMovement ()->DisableMovement ();

	//Anim
	if (bossAnim)
	{
		bossAnim->SetIsBossDead ( true );
		bossAnim->PlayBossDeathMontage ();
	}

	//TODO : Death Logic (Ragdoll , Destroy Actor after delay, Rewards, UI , etc)
	AK_Player* player = Cast<AK_Player> ( UGameplayStatics::GetPlayerCharacter ( GetWorld () , 0 ) );
	if (player)
	{
		player->ShowResultUI ( true );
	}
}
