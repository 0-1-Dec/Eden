// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EEnemyHPBarWidget.h"

void UEEnemyHPBarWidget::BindStatComponent(UECharacterStatComponent* InStatComp)
{
    if(!InStatComp) return;

    CurrentStatComp = InStatComp;

    InStatComp->OnHpChanged.AddDynamic(this,&UEEnemyHPBarWidget::UpdateHpBar);
    UpdateHpBar(InStatComp->GetCurrentHp());
}

void UEEnemyHPBarWidget::UpdateHpBar(float NewHp)
{
    if(CurrentStatComp.IsValid() && HpProgressBar)
    {
        float Ratio = NewHp / CurrentStatComp->GetMaxHp();
        HpProgressBar->SetPercent(Ratio);
    }
}