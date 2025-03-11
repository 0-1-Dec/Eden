// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class EDEN_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Weapon)
	FText WeaponName;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = WeaponStat)
	float BaseDamage;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = WeaponStat)
	float AttackRange;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = WeaponStat)
	float AttackSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = WeaponStat)
		UAnimMontage* AttackMontage;
};
