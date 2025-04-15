// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/EComboActionData.h"
#include "Engine/DataAsset.h"
#include "Item/EWeaponType.h"
#include "EWeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class EDEN_API UEWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Weapon)
	EWeaponType Weapon;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = WeaponStat)
	float BaseDamage;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = WeaponStat)
	float AttackRange;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = WeaponStat)
	float AttackSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = WeaponStat)
	float AttackRadius;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = WeaponStat)
	TObjectPtr<UEComboActionData> ComboActionData;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = WeaponStat)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = WeaponStat)
	UAnimMontage* SkillMontage;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = WeaponStat)
	UAnimMontage* EquipMontage;
};
