// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "CharacterStat/ECharacterStatComponent.h"
#include "EHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API UEHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UEHUDWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION(BlueprintCallable)
	void BindStatComponent(UECharacterStatComponent* InStatComp);

	void UpdateHpBar(float NewCurrentHp);
	
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
