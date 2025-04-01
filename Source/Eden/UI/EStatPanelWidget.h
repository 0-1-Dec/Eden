// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStat/ECharacterStatComponent.h"
#include "EStatPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API UEStatPanelWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay();

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Attack;

    int32 atk;

    /*
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Defense;
    */

    UPROPERTY(meta = (BindWidget))
    class UButton* AddAttackButton;

    UFUNCTION()
    void OnAddAttackClicked();

    void UpdateDisplay();

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* RemainStatPoint;
    
    int32 sp;

private:
    TWeakObjectPtr<UECharacterStatComponent> CurrentStatComp;

public:
    void BindStatComponent(UECharacterStatComponent* StatComp);
};
