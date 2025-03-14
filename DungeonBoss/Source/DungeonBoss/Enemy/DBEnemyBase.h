// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameData/DBEnemyStat.h"
#include "Interface/DBEnemyAIInterface.h"
#include "Enemy/DBEnemy_ArmoredBoss.h"
#include "DBEnemyBase.generated.h"

UCLASS()
class DUNGEONBOSS_API ADBEnemyBase : public ADBEnemy_ArmoredBoss, public IDBEnemyAIInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	//ADBEnemyBase();
	ADBEnemyBase();

//Component Section
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

//AI Section
protected:
	virtual void NotifyComboActionEnd() override;

//Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	class UDBEnemyStatComponent* Stat;

//IDBEnemyAIInterface
protected:
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAIEnemyAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

//Damage Section
protected:
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
