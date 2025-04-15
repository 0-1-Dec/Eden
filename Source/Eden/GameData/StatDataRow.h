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
    float ExpToNextLevel;

    UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Stat)
    int32 BaseAttack;

    UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Stat)
    int32 BaseDefense;

    UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Stat)
    int32 BaseCriticalChance;

    UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Stat)
    int32 BaseCriticalDamage;

    UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Stat)
    int32 BaseMaxHP;


    UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Stat)
    int32 StatPoint;
};