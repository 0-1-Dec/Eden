#pragma once

#include "GameData/EItemDataAsset.h"
#include "EInventorySlot.generated.h"

USTRUCT(BlueprintType)
struct FEInventorySlot
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Slot)
	UEItemDataAsset* ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Slot)
	int32 Quantity;

	FEInventorySlot() : ItemData(nullptr), Quantity(0) {}
};
