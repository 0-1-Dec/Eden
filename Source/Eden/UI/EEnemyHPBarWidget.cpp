// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EEnemyHPBarWidget.h"

UEEnemyHPBarWidget::UEEnemyHPBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    MaxHp = -1;
}

void UEEnemyHPBarWidget::NativeConstruct()
{
    Super::NativeConstruct();

    HpProgressBar = Cast<UProgressBar>(GetWidgetFromName("HpProgressBar"));
}

void UEEnemyHPBarWidget::BindStatComponent(UECharacterStatComponent* InStatComp)
{
    if(!InStatComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("InStat Component Error"));
        return;
    }

    CurrentStatComp = InStatComp;

    InStatComp->OnHpChanged.AddUObject(this,&UEEnemyHPBarWidget::UpdateHpBar);
    UpdateHpBar(InStatComp->GetCurrentHp());
}

void UEEnemyHPBarWidget::UpdateHpBar(float NewHp)
{
    MaxHp = CurrentStatComp->GetMaxHp();
    CurrentHp = NewHp;

    if (HpProgressBar)
    {
        HpProgressBar->SetPercent(CurrentHp / MaxHp);
    }
    
    // if(CurrentStatComp.IsValid() && HpProgressBar)
    // {
    //     float Ratio = NewHp / CurrentStatComp->GetMaxHp();
    //     UE_LOG(LogTemp, Warning, TEXT("Hp : %f %%"), Ratio * 100);
    //     HpProgressBar->SetPercent(Ratio);
    // }
}