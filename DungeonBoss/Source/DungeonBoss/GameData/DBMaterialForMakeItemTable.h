#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DBMaterialForMakeItemTable.generated.h"

USTRUCT(BlueprintType)
struct FDBMaterialForMakeItemTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDBMaterialForMakeItemTable() : Name(""), NeedMaterial(""), NeedMaterialCount("") {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FString NeedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FString NeedMaterialCount;
};
