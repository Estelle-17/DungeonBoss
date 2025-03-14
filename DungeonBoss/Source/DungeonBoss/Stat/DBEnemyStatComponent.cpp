// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/DBEnemyStatComponent.h"
#include "Net/UnrealNetwork.h"
#include "DungeonBoss.h"

// Sets default values for this component's properties
UDBEnemyStatComponent::UDBEnemyStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;

	//Component Replicates º¯°æ
	SetIsReplicatedByDefault(true);
}

void UDBEnemyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	FDBEnemyStat EnemyStat;
	EnemyStat.MaxHp = 100.0f;
	EnemyStat.Attack = 10.0f;
	EnemyStat.MoveSpeed = 1.0f;

	AddBaseStat(EnemyStat);
	SetHp(BaseStat.MaxHp);
}

void UDBEnemyStatComponent::SetBaseStat(const FDBEnemyStat* InBaseStat)
{
	BaseStat.MaxHp = InBaseStat->MaxHp;
	BaseStat.Attack = InBaseStat->Attack;
	BaseStat.AttackRange = InBaseStat->AttackRange;
	BaseStat.Defense = InBaseStat->Defense;
	BaseStat.MoveSpeed = InBaseStat->MoveSpeed;

	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
}

float UDBEnemyStatComponent::ApplyDamage(float InDamage)
{
	DB_LOG(LogDBNetwork, Log, TEXT("Damage %f In"), InDamage);

	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

// Called when the game starts
void UDBEnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UDBEnemyStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);

	DB_LOG(LogDBNetwork, Log, TEXT("CurrentHp : %f"), CurrentHp);

	OnHpChanged.Broadcast(CurrentHp);
}

void UDBEnemyStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDBEnemyStatComponent, CurrentHp);
}

void UDBEnemyStatComponent::OnRep_CurrentHp()
{
	OnHpChanged.Broadcast(CurrentHp);

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
}

