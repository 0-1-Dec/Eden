// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EInventorySlot.h"
#include "Blueprint/DragDropOperation.h"
#include "EInventoryDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API UEInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	FEInventorySlot DraggedSlot;

	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	int32 SourceSlotIndex;
};
