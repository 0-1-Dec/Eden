// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ECharacterStatComponent.h"
#include "ECharacterStatComponent.h"

// Sets default values for this component's properties
UECharacterStatComponent::UECharacterStatComponent()
{
	AttackRadius = 50.0f;

	bWantsInitializeComponent = true;
}

void UECharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetHp(200.f);
}

float UECharacterStatComponent::ApplyDamage(float InDamge)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamge, 0, InDamge);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UECharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, 200.f);

	OnHpChanged.Broadcast(CurrentHp);
}
