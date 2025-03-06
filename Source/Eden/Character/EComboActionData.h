// 프로젝트 설정의 Description 페이지에서 저작권 정보를 작성하세요.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EComboActionData.generated.h"

/**
 * UEComboActionData는 콤보 공격 애니메이션에 필요한 데이터를 저장하는 Data Asset입니다.
 * 이 클래스는 콤보 애니메이션 섹션 이름의 접두사, 최대 콤보 횟수, 애니메이션 프레임 레이트,
 * 각 콤보 단계에서 유효한 프레임 수를 저장하여 콤보 시스템의 타이밍을 관리하는데 사용됩니다.
 */
UCLASS()
class EDEN_API UEComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// 생성자: 기본 초기화 작업을 수행합니다.
	UEComboActionData();

	// MontageSectionNamePrefix:
	// 콤보 애니메이션의 각 섹션 이름에 사용할 접두사를 지정합니다.
	// 예를 들어 "Combo"로 설정하면, 애니메이션 섹션 이름은 "Combo1", "Combo2", ... 와 같이 생성될 수 있습니다.
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	// MaxComboCount:
	// 최대 콤보 횟수를 지정합니다.
	// 예: 최대 3회 콤보 가능하면 3으로 설정합니다.
	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

	// FrameRate:
	// 애니메이션의 프레임 레이트를 지정합니다.
	// 이 값은 콤보 입력 타이밍 계산 및 효과 시간 산정에 사용됩니다.
	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;

	// EffectiveFrameCount:
	// 각 콤보 단계별로 유효한 프레임 수를 저장하는 배열입니다.
	// 이 배열의 각 값은 해당 콤보 단계에서 입력이 유효한 시간을(프레임 단위로) 결정하는 데 사용됩니다.
	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;
};
