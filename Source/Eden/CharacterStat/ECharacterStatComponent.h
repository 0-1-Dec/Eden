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
	// Hp ���� �� �׾��� �� �̺�Ʈ�� ����ϴ� ��������Ʈ
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

	// �ǰݽ� hp ������ ���� �ζ��� �Լ� �� �Լ�
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }
	float ApplyDamage(float InDamge);

protected:
	// �ǰݽ� hp ������ ���� �Լ� �� ��� ����
	void SetHp(float NewHp);

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	// ���� ��Ÿ�
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float AttackRadius;
};
