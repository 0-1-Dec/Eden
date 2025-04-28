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

    virtual void NativeConstruct();

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BonusMaxHPTxt;

    UPROPERTY(meta = (BindWidget))
    class UButton* AddMaxHPBtn;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BonusAttackTxt;

    UPROPERTY(meta = (BindWidget))
    class UButton* AddAttackBtn;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BonusDefenseTxt;

    UPROPERTY(meta = (BindWidget))
    class UButton* AddDefenseBtn;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BonusCriticalChanceTxt;

    UPROPERTY(meta = (BindWidget))
    class UButton* AddCriticalChanceBtn;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BonusCriticalDamageTxt;

    UPROPERTY(meta = (BindWidget))
    class UButton* AddCriticalDamageBtn;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* FinalMaxHPTxt;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* FinalAttackTxt;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* FinalDefenseTxt;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* FinalCriticalChanceTxt;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* FinalCriticalDamageTxt;

    UFUNCTION(BlueprintCallable)
    void OnAddButtonClicked(UButton* ClickedBtn);

    UFUNCTION(BlueprintCallable)
    void UpdateDisplay();

    UFUNCTION(BlueprintCallable)
    void HandleMaxHPClick() { OnAddButtonClicked(AddMaxHPBtn);}

    UFUNCTION(BlueprintCallable)
    void HandleAttackClick() { OnAddButtonClicked(AddAttackBtn);}

    UFUNCTION(BlueprintCallable)
    void HandleDefenseClick() {OnAddButtonClicked(AddDefenseBtn);}

    UFUNCTION(BlueprintCallable)
    void HandleCriticalChanceClick() {OnAddButtonClicked(AddCriticalChanceBtn);}

    UFUNCTION(BlueprintCallable)
    void HandleCriticalDamageClick() {OnAddButtonClicked(AddCriticalDamageBtn);}

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* RemainStatPoint;

private:
    TWeakObjectPtr<UECharacterStatComponent> CurrentStatComp;

public:
    void BindStatComponent(UECharacterStatComponent* StatComp);
};
