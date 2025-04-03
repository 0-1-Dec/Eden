// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EHpBarWidget.h"

#include "Components/ProgressBar.h"
#include "Interface/ECharacterWidgetInterface.h"

UEHpBarWidget::UEHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1;
}

void UEHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpProgressBar")));
	ensure(HpProgressBar);

	if (IECharacterWidgetInterface* CharacterWidget = Cast<IECharacterWidgetInterface>(OwningActor))
	{
		CharacterWidget->SetUpCharacterWidget(this);
	}
}

void UEHpBarWidget::BindStatComponent(UECharacterStatComponent* InStatComp)
{
	if (InStatComp)
	{
		CurrentStatComp = InStatComp;
	}
}

void UEHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	MaxHp = CurrentStatComp->GetMaxHp();
	CurrentHp = NewCurrentHp;
	
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}
}