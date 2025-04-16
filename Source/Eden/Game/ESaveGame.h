// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CharacterStatSnapshot.h"
#include "Inventory/EInventorySlot.h"
#include "ESaveGame.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API UESaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FCharacterStatSnapshot StatSnapshot;
	
	UPROPERTY()
	TArray<FEInventorySlot> InventorySnapshot;
};
