// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/DBEnemyStat.h"
#include "DBEnemyStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEnemyHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEnemyStatChangedDelegate, const FDBEnemyStat& /*BaseStat*/, const FDBEnemyStat& /*ModifierStat*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONBOSS_API UDBEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDBEnemyStatComponent();
protected:
	// Called when the game starts
	virtual void InitializeComponent() override;

public:
	FOnEnemyHpZeroDelegate OnHpZero;
	FOnEnemyHpChangedDelegate OnHpChanged;
	FOnEnemyStatChangedDelegate OnStatChanged;

	FORCEINLINE void AddBaseStat(const FDBEnemyStat& AddBaseStat) { BaseStat = BaseStat + AddBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void SetBaseStat(const FDBEnemyStat& InBaseStat) { BaseStat = InBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	void SetBaseStat(const FDBEnemyStat* InBaseStat);
	FORCEINLINE void SetModifierStat(const FDBEnemyStat& InModifierStat) { ModifierStat = InModifierStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }

	FORCEINLINE const FDBEnemyStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FDBEnemyStat& GetModifierStat() const { return ModifierStat; }
	FORCEINLINE FDBEnemyStat GetTotalStat() const { return BaseStat + ModifierStat; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE void HealHp(float HealAmount) { CurrentHp = FMath::Clamp(CurrentHp + HealAmount, 0, GetTotalStat().MaxHp); OnHpChanged.Broadcast(CurrentHp); }
	float ApplyDamage(float InDamage);

protected:
	void SetHp(float NewHp);

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHp, Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FDBEnemyStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FDBEnemyStat ModifierStat;

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_CurrentHp();
};
