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

	SkillImg = Cast<UImage>(GetWidgetFromName(TEXT("SkillIcon")));
	ensure(SkillImg);

	SkillCoolDownImg = Cast<UImage>(GetWidgetFromName(TEXT("SkillCoolDown")));
	ensure(SkillCoolDownImg);

	SkillImg->SetBrushFromTexture(OneSkillIcon);

	if (IECharacterHUDInterface* HUDPawn = Cast<IECharacterHUDInterface>(GetOwningPlayerPawn()))
	{
		HUDPawn->SetupHUDWidget(this);
	}
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
	
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}
}

void UEHUDWidget::UpdateSkillIcon(EWeaponType CurrentWeapon)
{
	if (CurrentWeapon == EWeaponType::OneHanded)
	{
		SkillImg->SetBrushFromTexture(OneSkillIcon);
	}
	else if (CurrentWeapon == EWeaponType::BothHanded)
	{
		SkillImg->SetBrushFromTexture(BothSkillIcon);
	}
	else
	{
		SkillImg->SetBrushFromTexture(BowSkillIcon);
	}
}

void UEHUDWidget::ResetCooldown()
{
	ElapsedTime = 0;
	bIsCooldownActive = false;
}

