
#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	OneHanded UMETA(DisplayName = "OneHanded"),
	Bow UMETA(DisplayName = "Bow"),
	BothHanded UMETA(DisplayName = "BothHanded")
};