// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "CharacterStat/ECharacterStatComponent.h"
#include "Components/Image.h"
#include "Item/EWeaponType.h"
#include "EHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API UEHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UEHUDWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void BindStatComponent(UECharacterStatComponent* InStatComp);

	void UpdateHpBar(float NewCurrentHp);

	void UpdateSkillIcon(EWeaponType CurrentWeapon);

	void ResetCooldown();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCooldownActive = false;
	
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillIcon)
	UTexture2D* BowSkillIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillIcon)
	UTexture2D* OneSkillIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillIcon)
	UTexture2D* BothSkillIcon;

	UPROPERTY()
	TObjectPtr<class UImage> SkillImg;

	UPROPERTY()
	TObjectPtr<class UImage> SkillCoolDownImg;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float CoolDown = 5.0f;

	UPROPERTY()
	float ElapsedTime = 0.f;

private:
	TWeakObjectPtr<UECharacterStatComponent> CurrentStatComp;
};

