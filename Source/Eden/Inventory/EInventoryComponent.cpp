// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/EInventoryComponent.h"

// Sets default values for this component's properties
UEInventoryComponent::UEInventoryComponent()
{
	
}


// Called when the game starts
void UEInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitializeInventory(MaxSlots);
}

void UEInventoryComponent::InitializeInventory(int32 InSlotCount)
{
	if (InSlotCount <= 0)
	{
		InSlotCount = 1;
	}

	
	
	Slots.SetNum(InSlotCount);
}

bool UEInventoryComponent::AddItem(UEItemDataAsset* ItemData, int32 Quantity)
{
	if (!ItemData || Quantity <= 0)
	{
		return false;
	}

	bool bAddedItem = false;
	bool bCanStack = ItemData->bCanStack;
	int32 MaxStackSize = ItemData->MaxStackSize;

	if (bCanStack)
	{
		for (int32 i = 0; i < Slots.Num(); i++)
		{
			if (Slots[i].ItemData == ItemData && Slots[i].Quantity < MaxStackSize)
			{
				int32 SpaceLeft = MaxStackSize - Slots[i].Quantity;
				int32 AmountToAdd = FMath::Min(SpaceLeft, Quantity);

				Slots[i].Quantity += AmountToAdd;
				
				Quantity -= AmountToAdd;

				if (Quantity <= 0)
				{
					bAddedItem = true;
					OnInventoryChanged.Broadcast();
					return true;
				}
			}
		}
	}

	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i].ItemData == nullptr || Slots[i].Quantity <= 0)
		{
			Slots[i].ItemData = ItemData;

			if (bCanStack)
			{
				int32 AmountToAdd = FMath::Min(Quantity, MaxStackSize);
				Slots[i].Quantity = AmountToAdd;
				Quantity -= AmountToAdd;
			}
			else
			{
				Slots[i].Quantity = 1;
				Quantity -= 1;
			}

			if (Quantity <= 0)
			{
				OnInventoryChanged.Broadcast();
				return true;
			}

			bAddedItem = true;
		}
	}

	if (bAddedItem)
	{
		OnInventoryChanged.Broadcast();
		return true;
	}

	return (Quantity <= 0);
}

bool UEInventoryComponent::RemoveItem(UEItemDataAsset* ItemData, int32 Quantity)
{
	if (!ItemData || Quantity <= 0)
	{
		return false;
	}

	bool bRemovedItem = false;
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i].ItemData == ItemData && Slots[i].Quantity > 0)
		{
			int32 RemoveAmount = FMath::Min(Quantity, Slots[i].Quantity);
			Slots[i].Quantity -= RemoveAmount;
			Quantity -= RemoveAmount;

			if (Slots[i].Quantity <= 0)
			{
				Slots[i].ItemData = nullptr;
				Slots[i].Quantity = 0;
			}

			bRemovedItem = true;

			if (Quantity <= 0)
			{
				return true;
			}
		}
	}

	if (bRemovedItem)
	{
		OnInventoryChanged.Broadcast();
		return true;
	}

	return false;
}

FEInventorySlot UEInventoryComponent::GetSlot(int32 SlotIndex)
{
	if (Slots.IsValidIndex(SlotIndex))
	{
		return Slots[SlotIndex];
	}
	return FEInventorySlot();
}

void UEInventoryComponent::SetSlotInfo(int32 SlotIndex, const FEInventorySlot& NextSlot)
{
	if (Slots.IsValidIndex(SlotIndex))
	{
		Slots[SlotIndex] = NextSlot;
		OnInventoryChanged.Broadcast();
	}
}

void UEInventoryComponent::MoveItem(int32 SourceSlotIndex, int32 TargetSlotIndex)
{
	if (!Slots.IsValidIndex(SourceSlotIndex) || !Slots.IsValidIndex(TargetSlotIndex))
	{
		return;
	}

	if (Slots[TargetSlotIndex].ItemData == nullptr || Slots[TargetSlotIndex].Quantity <= 0)
	{
		Slots[TargetSlotIndex] = Slots[SourceSlotIndex];
		Slots[SourceSlotIndex].ItemData = nullptr;
		Slots[SourceSlotIndex].Quantity = 0;
		OnInventoryChanged.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("Item moved"));
	}
}

void UEInventoryComponent::SwapItems(int32 SourceSlotIndex, int32 TargetSlotIndex)
{
	if (!Slots.IsValidIndex(SourceSlotIndex) || !Slots.IsValidIndex(TargetSlotIndex))
	{
		return;
	}

	FEInventorySlot Temp = Slots[SourceSlotIndex];
	Slots[SourceSlotIndex] = Slots[TargetSlotIndex];
	Slots[TargetSlotIndex] = Temp;
	OnInventoryChanged.Broadcast();
	
	UE_LOG(LogTemp, Warning, TEXT("Item swapped Source : %d, Target : %d"), SourceSlotIndex, TargetSlotIndex);
}

void UEInventoryComponent::DebugPrintInventory()
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i].ItemData)
		{
			// ItemData->GetName()을 통해 아이템 이름을 가져올 수 있다고 가정합니다.
			UE_LOG(LogTemp, Warning, TEXT("Slot %d: %s, Quantity: %d"), i, *Slots[i].ItemData->GetName(), Slots[i].Quantity);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Slot %d: Empty"), i);
		}
	}
}


