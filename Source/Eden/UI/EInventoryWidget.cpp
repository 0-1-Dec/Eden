// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EInventoryWidget.h"
#include "EInventorySlotWidget.h"
#include "Character/ECharacterPlayer.h"
#include "Components/GridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Inventory/EInventoryDragDropOperation.h"

void UEInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!LinkedInventory)
	{
		if (APlayerController* PC = GetOwningPlayer())
		{
			if (AECharacterPlayer* Character = Cast<AECharacterPlayer>(PC->GetPawn()))
			{
				LinkedInventory = Character->InventoryComponent;
			}
		}
	}

	CreateInventorySlots();
	RefreshInventory();

	if (LinkedInventory)
	{
		LinkedInventory->OnInventoryChanged.AddDynamic(this, &UEInventoryWidget::HandleInventoryChanged);
	}
}

bool UEInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UEInventoryDragDropOperation* DragOp = Cast<UEInventoryDragDropOperation>(InOperation);
	if (DragOp)
	{
		FVector2D LocalDropPosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());

		if (!InGeometry.IsUnderLocation(InDragDropEvent.GetScreenSpacePosition()))
		{
			if (LinkedInventory)
			{
				LinkedInventory->RemoveItem(DragOp->DraggedSlot.ItemData, 1);
			}

			if (APlayerController* PC = GetOwningPlayer())
			{
				FVector2D ScreenPos = InDragDropEvent.GetScreenSpacePosition();
				FVector WorldLocation, WorldDirection;
				PC->DeprojectScreenPositionToWorld(ScreenPos.X, ScreenPos.Y, WorldLocation, WorldDirection);

				FVector SpawnLocation = WorldLocation;
				FRotator SpawnRotation = FRotator::ZeroRotator;
				
			}
		}
		else
		{
			bool bFoundSlot = false;
			UEInventorySlotWidget* TargetSlotWidget = nullptr;
			if (SlotsGrid)
			{
				int32 ChildCount = SlotsGrid->GetChildrenCount();
				for (int32 i = 0; i < ChildCount; i++)
				{
					UWidget* Child = SlotsGrid->GetChildAt(i);

					if (Child && Child == DragOp->DefaultDragVisual)
					{
						continue;
					}
					
					if (Child && Child->GetCachedGeometry().IsUnderLocation(InDragDropEvent.GetScreenSpacePosition()))
					{
						bFoundSlot = true;
						TargetSlotWidget = Cast<UEInventorySlotWidget>(Child);
						break;
					}
				}
			}

			if (bFoundSlot && TargetSlotWidget)
			{
				if (TargetSlotWidget->IsEmpty())
				{
					if (LinkedInventory)
					{
						int32 SourceSlotIndex = DragOp->SourceSlotIndex;
						int32 TargetSlotIndex = TargetSlotWidget->GetSlotIndex();
						LinkedInventory->MoveItem(SourceSlotIndex, TargetSlotIndex);
					}
				}
				else
				{
					if (LinkedInventory)
					{
						int32 SourceSlotIndex = DragOp->SourceSlotIndex;
						int32 TargetSlotIndex = TargetSlotWidget->GetSlotIndex();
						LinkedInventory->SwapItems(SourceSlotIndex, TargetSlotIndex);
					}
				}
			}
			else
			{
				if (LinkedInventory)
				{
					LinkedInventory->RemoveItem(DragOp->DraggedSlot.ItemData, 1);
				}

				if (APlayerController* PC = GetOwningPlayer())
				{
					FVector2D ScreenPos = InDragDropEvent.GetScreenSpacePosition();
					FVector WorldLocation, WorldDirection;
					PC->DeprojectScreenPositionToWorld(ScreenPos.X, ScreenPos.Y, WorldLocation, WorldDirection);
					FVector SpawnLocation = WorldLocation;
					FRotator SpawnRotation = FRotator::ZeroRotator;
				}
			}
		}
		return true;
	}
	return Super::NativeOnDrop(InGeometry,InDragDropEvent,InOperation);
}

void UEInventoryWidget::CreateInventorySlots()
{
	if (!SlotsGrid || !SlotWidgetClass || !LinkedInventory)
		return;
	
	SlotsGrid->ClearChildren();

	int32 SlotCount = LinkedInventory->GetSlotCount();
	int32 Columns = 5;
	for (int32 i = 0; i < SlotCount; i++)
	{
		UEInventorySlotWidget* SlotWidget = CreateWidget<UEInventorySlotWidget>(GetWorld(), SlotWidgetClass, FName(*FString::Printf(TEXT("Slot %i"), i)));
		if (!SlotWidget) continue;

		SlotWidget->SetSlotIndex(i);
		
		UUniformGridSlot* GridSlot = SlotsGrid->AddChildToUniformGrid(SlotWidget);
		if (GridSlot)
		{
			GridSlot->SetRow(i / Columns);
			GridSlot->SetColumn(i % Columns);
			GridSlot->SetHorizontalAlignment(HAlign_Fill);
			GridSlot->SetVerticalAlignment(VAlign_Fill);
		}
	}
}

void UEInventoryWidget::RefreshInventory()
{
	if (!SlotsGrid || !LinkedInventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("Something is wrong"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Inventory refreshed"));

	int32 SlotCount = LinkedInventory->GetSlotCount();
	int32 NumChildren = SlotsGrid->GetChildrenCount();

	for (int32 i = 0; i < SlotCount && i < NumChildren; i++)
	{
		FEInventorySlot SlotData = LinkedInventory->GetSlot(i);

		UWidget* Child = SlotsGrid->GetChildAt(i);
		if (UEInventorySlotWidget* SlotWBP = Cast<UEInventorySlotWidget>(Child))
		{
			SlotWBP->SetSlotData(SlotData);
		}
	}
}

void UEInventoryWidget::HandleInventoryChanged()
{
	RefreshInventory();
}
