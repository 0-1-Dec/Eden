#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EDropDataStruct.generated.h"

class UEItemDataAsset;

USTRUCT(BlueprintType)
struct FDropEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    TSoftObjectPtr<UEItemDataAsset> Item;

    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    float DropChance = 1.0f;

    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    int32 MinCount = 1;

    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    int32 MaxCount = 1;
};