// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EHUDWidget.h"
#include "Interface/ECharacterHUDInterface.h"
#include "EHpBarWidget.h"

UEHUDWidget::UEHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1;
}

void UEHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpVerticalBar")));
	ensure(HpProgressBar);

	if (IECharacterHUDInterface* HUDPawn = Cast<IECharacterHUDInterface>(GetOwningPlayerPawn()))
	{
		HUDPawn->SetupHUDWidget(this);
	}
}

void UEHUDWidget::BindStatComponent(UECharacterStatComponent* InStatComp)
{
	if (InStatComp)
	{
		CurrentStatComp = InStatComp;
	}
}

void UEHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	MaxHp = CurrentStatComp->GetMaxHp();
	CurrentHp = NewCurrentHp;
	
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}
}

