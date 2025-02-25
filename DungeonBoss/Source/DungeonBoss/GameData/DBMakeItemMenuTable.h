#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DBMakeItemMenuTable.generated.h"

USTRUCT(BlueprintType)
struct FDBMakeItemMenuTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDBMakeItemMenuTable() : Name(""), MenuID(""), IsChild(false), TexturePath("") {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FName MenuID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	bool IsChild;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FString TexturePath;
};
