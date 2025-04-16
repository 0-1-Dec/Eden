#pragma once

UENUM(BlueprintType)
enum class ECharacterStatType: uint8
{
	BonusMaxHP,
	BonusAttack,
	BonusDefence,
	BonusCriticalChance,
	BonusCriticalDamage,
	// 필요시 더 추가할 것
};
