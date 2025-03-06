// 프로젝트 설정의 Description 페이지에서 저작권 정보를 작성하세요.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ECharacterStatComponent.generated.h"

// HP가 0이 되었을 때 발생하는 이벤트 델리게이트 선언
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);

// HP가 변경될 때 호출되며, 변경된 HP 값을 전달하는 이벤트 델리게이트 선언
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float)

// UECharacterStatComponent 클래스는 캐릭터의 스탯(예: HP, 공격 사거리 등)을 관리하는 컴포넌트입니다.
// Blueprint에서 스폰이 가능하도록 meta 설정을 포함하고 있습니다.
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EDEN_API UECharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// 생성자: 컴포넌트의 기본 속성 값을 초기화합니다.
	UECharacterStatComponent();

protected:
	// 컴포넌트가 초기화될 때 호출되는 함수.
	// 초기화 작업이나 다른 컴포넌트와의 연동 작업을 수행할 수 있습니다.
	virtual void InitializeComponent() override;

public:
	// HP가 0이 되었을 때 호출되는 델리게이트 (예: 캐릭터 사망 처리)
	FOnHpZeroDelegate OnHpZero;

	// HP가 변경될 때마다 호출되는 델리게이트 (파라미터: 변경된 HP 값)
	FOnHpChangedDelegate OnHpChanged;

	// 인라인 함수: 현재 HP 값을 반환합니다.
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }

	// 인라인 함수: 공격 사거리 값을 반환합니다.
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }

	// ApplyDamage: 외부에서 데미지를 적용할 때 호출되는 함수.
	// 데미지 값(InDamage)을 받아 HP를 차감하고, 관련 이벤트를 발생시킵니다.
	float ApplyDamage(float InDamge);

protected:
	// SetHp: HP 값을 설정하는 내부 함수.
	// HP 변경 후, OnHpChanged 델리게이트를 호출하고, HP가 0이 되면 OnHpZero 델리게이트를 실행합니다.
	void SetHp(float NewHp);

	// 현재 HP 값.
	// Transient, VisibleInstanceOnly 속성은 런타임 동안에만 유효하며 에디터에서 볼 수 있도록 합니다.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	// 공격 사거리: 캐릭터의 공격 범위를 나타내는 값.
	// Transient, VisibleInstanceOnly 속성으로 설정되어 런타임 동안 값이 유지됩니다.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float AttackRadius;
};
