// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EHUDWidget.h"
#include "Interface/ECharacterHUDInterface.h"
#include "EHpBarWidget.h"

UEHUDWidget::UEHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1;
	CurrentLevel = 1;
	ExpToNextLevel = -1;
}

void UEHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpVerticalBar")));
	ensure(HpProgressBar);
	
	EnergyProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("EnergyVerticalBar")));
	ensure(EnergyProgressBar);

	ExpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ExpVerticalBar")));
	ensure(ExpProgressBar);

	SkillImg = Cast<UImage>(GetWidgetFromName(TEXT("SkillIcon")));
	ensure(SkillImg);

	SkillCoolDownImg = Cast<UImage>(GetWidgetFromName(TEXT("SkillCoolDown")));
	ensure(SkillCoolDownImg);

	PotionCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PotionCountText")));
	ensure(PotionCountText);

	SkillImg->SetBrushFromTexture(OneSkillIcon);

	if (IECharacterHUDInterface* HUDPawn = Cast<IECharacterHUDInterface>(GetOwningPlayerPawn()))
	{
		HUDPawn->SetupHUDWidget(this);
	}
	
	SkillImg->SetToolTipText(FText::FromString(OneSkillTooltip));
	
	HpProgressBar->SetToolTipText(FText::FromString(HpTooltip));
}

void UEHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);

	if (bIsCooldownActive)
	{
		ElapsedTime += InDeltaTime;
		if (ElapsedTime > CoolDown)
		{
			ElapsedTime = CoolDown;
			bIsCooldownActive = false;
		}

		float _pct = 1.0f - (ElapsedTime / CoolDown);

		if (SkillCoolDownImg)
		{
			UMaterialInstanceDynamic* DynamicSkillIcon = Cast<UMaterialInstanceDynamic>(SkillCoolDownImg->GetDynamicMaterial());
			if (DynamicSkillIcon)
			{
				DynamicSkillIcon->SetScalarParameterValue("Percent", _pct);
			}
		}
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
	
	HpTooltip = FString::Printf(TEXT("체력 : %.1f / %.1f"), CurrentHp, MaxHp);
	
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);

		HpProgressBar->SetToolTipText(FText::FromString(HpTooltip));
	}
}

void UEHUDWidget::UpdateEnergyBar(float NewCurrentEnergy)
{
	CurrentEnergy = NewCurrentEnergy;

	if (EnergyProgressBar)
	{
		EnergyProgressBar->SetPercent(CurrentEnergy / MaxEnergy);
	}
}

void UEHUDWidget::UpdateExpBar(float NewExp)
{
	CurrentExp = NewExp;
	ExpToNextLevel = CurrentStatComp->GetStatRow(CurrentLevel)->ExpToNextLevel;

	if (ExpProgressBar)
	{
		ExpProgressBar->SetPercent(CurrentExp / ExpToNextLevel);
	}
}

void UEHUDWidget::UpdateLevel()
{
	CurrentLevel++;
}

void UEHUDWidget::UpdatePotionCount(int32 NewPotionCount)
{
	PotionCountText->SetText(FText::AsNumber(NewPotionCount));
}

void UEHUDWidget::UpdateSkillIcon(EWeaponType CurrentWeapon)
{
	if (CurrentWeapon == EWeaponType::OneHanded)
	{
		SkillImg->SetBrushFromTexture(OneSkillIcon);
		
		SkillImg->SetToolTipText(FText::FromString(OneSkillTooltip));
	}
	else if (CurrentWeapon == EWeaponType::BothHanded)
	{
		SkillImg->SetBrushFromTexture(BothSkillIcon);

		SkillImg->SetToolTipText(FText::FromString(BothSkillTooltip));
	}
	else
	{
		SkillImg->SetBrushFromTexture(BowSkillIcon);

		SkillImg->SetToolTipText(FText::FromString(BowSkillTooltip));
	}
}

void UEHUDWidget::ResetCooldown()
{
	ElapsedTime = 0;
	bIsCooldownActive = false;
}

