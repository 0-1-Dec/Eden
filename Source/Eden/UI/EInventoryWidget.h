// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EInventorySlotWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "Inventory/EInventoryComponent.h"
#include "Components/UniformGridPanel.h"
#include "EInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API UEInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = Inventory, meta=(ExposeOnSpawn=true))
	UEInventoryComponent* LinkedInventory;

	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* SlotsGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TSubclassOf<UEInventorySlotWidget> SlotWidgetClass;

protected:
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UFUNCTION(BlueprintCallable)
	void CreateInventorySlots();

	UFUNCTION(BlueprintCallable)
	void RefreshInventory();

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	void HandleInventoryChanged();
	
private:
	UPROPERTY(meta=(BindWidget))
	USizeBox* HeaderBox;

	FVector2D DragOffset;
	bool bDragging = false;
};
