// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/DBCharacterStat.h"
#include "DBCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FDBCharacterStat& /*BaseStat*/, const FDBCharacterStat& /*ModifierStat*/);

USTRUCT(Atomic, BlueprintType)
struct FCharacterEquipStats
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(Transient, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FDBCharacterStat WeaponStat;

	UPROPERTY(Transient, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FDBCharacterStat HeadStat;

	UPROPERTY(Transient, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FDBCharacterStat BodyStat;

	UPROPERTY(Transient, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FDBCharacterStat ShoesStat;

	UPROPERTY(Transient, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FDBCharacterStat EquipTotalStat;

	void SetEquipTotalStat()
	{
		EquipTotalStat = WeaponStat + HeadStat + BodyStat + ShoesStat;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONBOSS_API UDBCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDBCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void InitializeComponent() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

	FORCEINLINE void AddBaseStat(const FDBCharacterStat& AddBaseStat) { BaseStat = BaseStat + AddBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void SetBaseStat(const FDBCharacterStat& InBaseStat) { BaseStat = InBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }

	FORCEINLINE const FDBCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FDBCharacterStat& GetModifierStat() { return CharacterEquipStats.EquipTotalStat; }
	FORCEINLINE const FDBCharacterStat GetTotalStat() { return BaseStat + CharacterEquipStats.EquipTotalStat; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE void HealHp(float HealAmount) { CurrentHp = FMath::Clamp(CurrentHp + HealAmount, 0, GetTotalStat().MaxHp); OnHpChanged.Broadcast(CurrentHp); }
	float ApplyDamage(float InDamage);
	void SetModifierStat(FDBCharacterStat NewEquipStat, int32 ItemType);

protected:
	void SetHp(float NewHp);

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHp, Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FDBCharacterStat BaseStat;

	FCharacterEquipStats CharacterEquipStats;

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_CurrentHp();
};
