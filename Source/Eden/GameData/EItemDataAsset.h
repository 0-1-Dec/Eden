// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API UEItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	bool bCanStack = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 MaxStackSize = 1;
};
