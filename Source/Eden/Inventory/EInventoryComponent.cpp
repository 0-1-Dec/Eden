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
		InSlotCount = 25;
	}
	
	Slots.SetNum(InSlotCount);
	
	OnInventoryChanged.Broadcast();
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

bool UEInventoryComponent::RemoveItem(UEItemDataAsset* ItemData, int32 SlotIndex)
{
	if (!ItemData || SlotIndex < 0 || SlotIndex >= Slots.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Error In RemoveItem"));
		return false;
	}

	if (Slots[SlotIndex].ItemData && Slots[SlotIndex].Quantity > 0)
	{
		Slots[SlotIndex].Quantity -= 1;

		

		if (Slots[SlotIndex].Quantity <= 0)
		{
			Slots[SlotIndex].ItemData = nullptr;
			Slots[SlotIndex].Quantity = 0;
			OnInventoryChanged.Broadcast();
			return true;
		}

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
	}
}

void UEInventoryComponent::SwapItems(int32 SourceSlotIndex, int32 TargetSlotIndex)
{
	if (!Slots.IsValidIndex(SourceSlotIndex) || !Slots.IsValidIndex(TargetSlotIndex))
	{
		return;
	}
	
	if (Slots[SourceSlotIndex].ItemData == Slots[TargetSlotIndex].ItemData)
	{
		int32 Increment = FMath::Abs(Slots[TargetSlotIndex].Quantity - Slots[SourceSlotIndex].Quantity);
		if (Slots[TargetSlotIndex].ItemData->MaxStackSize < Slots[TargetSlotIndex].Quantity + Increment)
		{
			FEInventorySlot Temp = Slots[TargetSlotIndex];
			Slots[TargetSlotIndex].Quantity = Slots[SourceSlotIndex].Quantity;
			Slots[SourceSlotIndex].Quantity = Temp.Quantity;
		}
		else
		{
			Slots[TargetSlotIndex].Quantity += Increment;
			Slots[SourceSlotIndex].Quantity -= Increment;	
		}
		
		if (Slots[SourceSlotIndex].Quantity <= 0)
		{
			Slots[SourceSlotIndex].ItemData = nullptr;
			Slots[SourceSlotIndex].Quantity = 0;
		}
		
		OnInventoryChanged.Broadcast();
		return;
	}
	
	FEInventorySlot Temp = Slots[SourceSlotIndex];
	Slots[SourceSlotIndex] = Slots[TargetSlotIndex];
	Slots[TargetSlotIndex] = Temp;
	OnInventoryChanged.Broadcast();
}

void UEInventoryComponent::InitFromArray(const TArray<FEInventorySlot>& InSlots)
{
	Slots = InSlots;

	OnInventoryChanged.Broadcast();
}
