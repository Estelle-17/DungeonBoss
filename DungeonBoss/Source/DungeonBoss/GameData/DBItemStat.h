#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DBItemStat.generated.h"

USTRUCT(BlueprintType)
struct FDBItemStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDBItemStat() : Name(""), Rank(0.0f), MaxHp(0.0f), Attack(0.0f), Defense(0.0f), MovementSpeed(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Rank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;
};
