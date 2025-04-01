// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EStatPanelWidget.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStat/ECharacterStatComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

/*
void UEStatPanelWidget::BeginPlay(){

}

void UEStatPanelWidget::BindStatComponent(UECharacterStatComponent* StatComp)
{
	CurrentStatComp = StatComp;

	if(AddAttackButton)
	{
		AddAttackButton->OnClicked.AddDynamic(this,&UEStatPanelWidget::OnAddAttackClicked);
	}

	// 초기값 설정
	atk = 0;
	sp = StatComp ? StatComp->StatPoints : 0;

	UpdateDisplay();
}

void UEStatPanelWidget::OnAddAttackClicked()
{
	if(!CurrentStatComp.IsValid()) return;
	if(sp <= 0) return;

	atk += 1;
	sp -= 1;

	UpdateDisplay();
}

void UEStatPanelWidget::UpdateDisplay()
{
	if(Attack)
	{
		Attack->SetText(FText::Format(FText::FromString(TEXT("+{0}")),FText::AsNumber(atk)));
	}
	if(RemainStatPoint)
	{
		RemainStatPoint->SetText(FText::AsNumber(sp));
	}
}
*/