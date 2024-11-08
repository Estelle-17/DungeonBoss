// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/DBCharacterStatComponent.h"
#include "Net/UnrealNetwork.h"
#include "DungeonBoss.h"

// Sets default values for this component's properties
UDBCharacterStatComponent::UDBCharacterStatComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;

	//Component Replicates º¯°æ
	SetIsReplicatedByDefault(true);
}


void UDBCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	FDBCharacterStat CharacterStat;
	CharacterStat.MaxHp = 100.0f;
	CharacterStat.Attack = 10.0f;
	CharacterStat.MovementSpeed = 1.0f;

	SetBaseStat(CharacterStat);
	SetHp(BaseStat.MaxHp);
	CharacterEquips.SetEquipTotalStat();
}

float UDBCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	DB_LOG(LogDBNetwork, Log, TEXT("Damage %f In"), ActualDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

// Called when the game starts
void UDBCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UDBCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);

	DB_LOG(LogDBNetwork, Log, TEXT("CurrentHp : %f"), CurrentHp);

	OnHpChanged.Broadcast(CurrentHp);
}

void UDBCharacterStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDBCharacterStatComponent, CurrentHp);
}

void UDBCharacterStatComponent::OnRep_CurrentHp()
{
	OnHpChanged.Broadcast(CurrentHp);

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
}


