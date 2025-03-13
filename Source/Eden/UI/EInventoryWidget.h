// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EInventorySlotWidget.h"
#include "Blueprint/UserWidget.h"
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
public:
	UFUNCTION()
	void HandleInventoryChanged();
};
