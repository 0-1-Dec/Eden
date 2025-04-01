// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EDropDataStruct.h"
#include "EDropDataAsset.generated.h"

UCLASS(BlueprintType)
class EDEN_API UEDropDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    int32 Exp = 0;

    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    int32 Gold = 0;

    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    TArray<FDropEntry> DropItems;
};
