// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EUserWidget.h"
#include "Components/TextBlock.h"
#include "CharacterStat/ECharacterStatComponent.h"
#include "EHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API UEHpBarWidget : public UEUserWidget
{
	GENERATED_BODY()

public:
	UEHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void BindStatComponent(UECharacterStatComponent* InStatComp);
	void UpdateHpBar(float NewCurrentHp);
	// FString GetHpStatText();

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;

private:
	TWeakObjectPtr<UECharacterStatComponent> CurrentStatComp;
};

