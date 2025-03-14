// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DBEnemyBase.h"
#include "Stat/DBEnemyStatComponent.h"
#include "DungeonBoss.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "UI/DBHpBarWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "AI/DBAIController.h"
#include "GameData/DBItemSingleton.h"
#include "MotionWarpingComponent.h"

// Sets default values
ADBEnemyBase::ADBEnemyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AI Setting
	AIControllerClass = ADBAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//몹의 회전 세팅
	bUseControllerRotationYaw = false;

	Tags.Add(FName("Enemy"));

	Stat = CreateDefaultSubobject<UDBEnemyStatComponent>(TEXT("EnemyStat"));
}

// Called when the game starts or when spawned
void ADBEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (Stat)
	{
		FName CurrentEnemyType = TEXT("ARMORED_BOSS");

		const FDBEnemyStat* EnemyStatDataTable = UDBItemSingleton::Get().GetLoadDBEnemyStatTableData(CurrentEnemyType);
		if (EnemyStatDataTable)
		{
			GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Green, FString::Printf(TEXT("Check %s Stat : %f"), *CurrentEnemyType.ToString(), EnemyStatDataTable->AttackRange));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Green, FString::Printf(TEXT("EnemyStat is nullptr")));
		}

		Stat->SetBaseStat(EnemyStatDataTable);
	}
}

void ADBEnemyBase::Tick(float DeltaTime)
{
	if (bCheckMotionWarping)
	{
		UpdateMotionWarpingRotation(TargetPawn->GetActorLocation());
	}
}

float ADBEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}


float ADBEnemyBase::GetAIDetectRange()
{
	return 5000.0f;
}

float ADBEnemyBase::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange;
}

float ADBEnemyBase::GetAITurnSpeed()
{
	return 5.0f;
}

void ADBEnemyBase::SetAIAttackDelegate(const FAIEnemyAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ADBEnemyBase::AttackByAI()
{
	PlaySwordAttackAction();
}

void ADBEnemyBase::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();

	OnAttackFinished.ExecuteIfBound();
}