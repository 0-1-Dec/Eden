// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStat/ECharacterStatComponent.h"
#include "ESaveGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API UESaveGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UESaveGameWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<UECharacterStatComponent> StatComponent;
	
	UPROPERTY()
	TObjectPtr<class UButton> SaveButton;

	UFUNCTION()
	void OnButtonClicked();
};
