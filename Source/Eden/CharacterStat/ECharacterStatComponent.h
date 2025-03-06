// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ECharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EDEN_API UECharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UECharacterStatComponent();

protected:
	virtual void InitializeComponent() override;

public:	
	// Hp 변경 및 죽었을 때 이벤트를 담당하는 델리게이트
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

	// 피격시 hp 관리를 위한 인라인 함수 및 함수
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }
	float ApplyDamage(float InDamge);

protected:
	// 피격시 hp 관리를 위한 함수 및 멤버 변수
	void SetHp(float NewHp);

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	// 공격 사거리
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float AttackRadius;
};
