// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EWeaponDataAsset.h"
#include "Engine/DataAsset.h"
#include "ENPCMeshDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FNPCMeshMaterialSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UAnimBlueprint> AnimBlueprint;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UEWeaponDataAsset> WeaponDataAsset;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere)
	TArray<FSoftObjectPath> MaterialPaths;
};

/**
 * 
 */
UCLASS()
class EDEN_API UENPCMeshDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FNPCMeshMaterialSet> NPCEntries;
};
