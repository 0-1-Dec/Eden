// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StatDataRow.generated.h"

USTRUCT(BlueprintType)
struct FStatDataRow: public FTableRowBase
{
    GENERATED_BODY();

public:
    UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Level)
    int32 Level;

    UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Level)
    int32 ExpToNextLevel;
};