// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EStatPanelWidget.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStat/ECharacterStatComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UEStatPanelWidget::BeginPlay(){

}

void UEStatPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateDisplay();
}

void UEStatPanelWidget::BindStatComponent(UECharacterStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	UpdateDisplay();
}

void UEStatPanelWidget::OnAddButtonClicked(UButton* ClickedBtn)
{

	if(!CurrentStatComp.IsValid()) return;

	UE_LOG(LogTemp,Warning,TEXT("OnAddButtonClicked called"));

	if(ClickedBtn)
		UE_LOG(LogTemp,Warning,TEXT("Clicked button: %s"),*ClickedBtn->GetName());
	if(AddAttackBtn)
		UE_LOG(LogTemp,Warning,TEXT("AddAttackBtn: %s"),*AddAttackBtn->GetName());


	if(CurrentStatComp->StatPoints <= 0) return;

	if(ClickedBtn == AddMaxHPBtn) CurrentStatComp->AddBonusStat(ECharacterStatType::BonusMaxHP);
	if(ClickedBtn == AddAttackBtn) {
		UE_LOG(LogTemp,Warning,TEXT("Button Match: AddAttackBtn clicked!")); CurrentStatComp->AddBonusStat(ECharacterStatType::BonusAttack);
	}
	if(ClickedBtn == AddDefenseBtn) CurrentStatComp->AddBonusStat(ECharacterStatType::BonusDefense);
	if(ClickedBtn == AddCriticalChanceBtn) CurrentStatComp->AddBonusStat(ECharacterStatType::BonusCriticalChance);
	if(ClickedBtn == AddCriticalDamageBtn) CurrentStatComp->AddBonusStat(ECharacterStatType::BonusCriticalDamage);

	CurrentStatComp->StatPoints--;
	UpdateDisplay();
}

void UEStatPanelWidget::UpdateDisplay()
{
	BonusMaxHPTxt->SetText(FText::AsNumber(CurrentStatComp->GetBonusStat(ECharacterStatType::BonusMaxHP)));
	BonusAttackTxt->SetText(FText::AsNumber(CurrentStatComp->GetBonusStat(ECharacterStatType::BonusAttack)));
	BonusDefenseTxt->SetText(FText::AsNumber(CurrentStatComp->GetBonusStat(ECharacterStatType::BonusDefense)));
	BonusCriticalChanceTxt->SetText(FText::AsNumber(CurrentStatComp->GetBonusStat(ECharacterStatType::BonusCriticalChance)));
	BonusCriticalDamageTxt->SetText(FText::AsNumber(CurrentStatComp->GetBonusStat(ECharacterStatType::BonusCriticalDamage)));
	RemainStatPoint->SetText(FText::AsNumber(CurrentStatComp->StatPoints));
}
