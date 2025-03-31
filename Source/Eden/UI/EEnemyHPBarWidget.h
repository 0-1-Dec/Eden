#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStat/ECharacterStatComponent.h"
#include "Components/ProgressBar.h"
#include "EEnemyHPBarWidget.generated.h"

UCLASS(Blueprintable,BlueprintType)
class EDEN_API UEEnemyHPBarWidget: public UUserWidget
{
    GENERATED_BODY()

public:
    UEEnemyHPBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void NativeConstruct() override;

public:
    UFUNCTION(BlueprintCallable)
    void BindStatComponent(UECharacterStatComponent* InStatComp);

    UFUNCTION()
    void UpdateHpBar(float NewHp);

protected:
    UPROPERTY()
    TObjectPtr<class UProgressBar> HpProgressBar;

    UPROPERTY()
    float CurrentHp;

    UPROPERTY()
    float MaxHp;

private:
    TWeakObjectPtr<UECharacterStatComponent> CurrentStatComp;
};