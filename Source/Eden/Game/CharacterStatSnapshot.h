#pragma once

#include "CoreMinimal.h"
#include "CharacterStatSnapshot.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStatSnapshot
{
	GENERATED_BODY()

public:
	/* 진행도 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite) int32  Level      = 1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite) float  Exp        = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite) int32  StatPoints = 3;

	/* 실시간 값 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float  CurrentHp  = 200.f;

	/* 보너스 스탯 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite) float  BonusMaxHp          = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite) float  BonusAttack         = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite) float  BonusDefence        = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite) float  BonusCriticalChance = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite) float  BonusCriticalDamage = 0.f;
};
