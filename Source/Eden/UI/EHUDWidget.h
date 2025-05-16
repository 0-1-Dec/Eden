// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
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
	void UpdateEnergyBar(float NewCurrentEnergy);
	void UpdateExpBar(float NewExp);
	void UpdateLevel();
	void UpdatePotionCount(int32 NewPotionCount);

	UFUNCTION()
	void UpdateSkillIcon(EWeaponType CurrentWeapon);

	void ResetCooldown();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCooldownActive = false;

// Hp 섹션
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;

// 기력 섹션
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> EnergyProgressBar;
	
	UPROPERTY()
	float CurrentEnergy;

	UPROPERTY()
	float MaxEnergy = 100.f;
	

// Exp 섹션
	UPROPERTY()
	TObjectPtr<class UProgressBar> ExpProgressBar;

	UPROPERTY()
	float CurrentExp;

	UPROPERTY()
	float ExpToNextLevel;

	UPROPERTY()
	float CurrentLevel;

// 포션 섹션
	UPROPERTY()
	TObjectPtr<class UTextBlock> PotionCountText;

	UPROPERTY()
	int32 CurrentPotionCount;

// 스킬 섹션
protected:
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
	float CoolDown = 5.0f;

	UPROPERTY()
	float ElapsedTime = 0.f;

private:
	TWeakObjectPtr<UECharacterStatComponent> CurrentStatComp;

	FString HpTooltip;
	FString OneSkillTooltip = TEXT("스킬 단축키 : E\n스킬 이름 : 회전 베기\n재사용 대기시간 : 3초\n스킬 설명 : 공중에서 한바퀴를 회전하며 적에게 큰 피해를 입힙니다.");
	FString BowSkillTooltip = TEXT("스킬 단축키 : E\n스킬 이름 : 프렐요드 폭우\n재사용 대기시간 : 3초\n스킬 설명 : 가장 가까운 적에게 3발의 유도 화살을 발사합니다.");
	FString BothSkillTooltip = TEXT("스킬 단축키 : E\n스킬 이름 : 대지 짓누르기\n재사용 대기시간 : 3초\n스킬 설명 : 땅을 힘껏 찍어 전방에 갈라진 대지를 만듭니다.");
};

