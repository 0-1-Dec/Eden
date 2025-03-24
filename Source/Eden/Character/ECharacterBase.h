// 프로젝트 설정의 Description 페이지에서 저작권 정보를 작성하세요.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/EAnimationAttackInterface.h"
#include "GameData/EWeaponDataAsset.h"
#include "ECharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponDataChanged, UEWeaponDataAsset*, NewWeaponData);

// AECharacterBase 클래스는 ACharacter를 상속받아 게임 캐릭터의 기본 동작을 구현하며,
// IEAnimationAttackInterface를 구현하여 애니메이션 공격 관련 기능을 제공합니다.
UCLASS()
class EDEN_API AECharacterBase : public ACharacter, public IEAnimationAttackInterface
{
	GENERATED_BODY()

public:
	// 생성자: 이 캐릭터 클래스의 기본 속성 값들을 초기화합니다.
	AECharacterBase();

	virtual void PostInitializeComponents() override;

public:
	FOnWeaponDataChanged OnWeaponDataChanged;

	// =========================================================================================
	// 무기 상태 관리
	// =========================================================================================
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Weapon,meta=(AllowPrivateAccess="true"))
	UEWeaponDataAsset* CurrentWeaponData;

	void SetWeaponData(UEWeaponDataAsset* NewWeaponData);

	// =========================================================================================
	// 콤보 애니메이션 섹션
	// =========================================================================================
protected:
	// 블루프린트나 에디터에서 수정할 수 있으며, 콤보 액션에 사용되는 애니메이션 몽타주를 저장합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	// ComboActionData:
	// 콤보 공격과 관련된 데이터를 저장하는 객체로, 에디터에서 설정 가능하지만 블루프린트에서는 읽기 전용입니다.
	// Meta 속성 AllowPrivateAccess는 내부 접근 권한을 허용합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UEComboActionData> ComboActionData;

	// 콤보 명령을 처리하는 함수: 플레이어의 입력에 따른 콤보 연계 처리를 수행합니다.
	void ProcessComboCommand();

	// 콤보 액션 시작 함수: 콤보 공격의 시작 시 호출되어 애니메이션 등을 초기화합니다.
	void ComboActionBegin();

	// 콤보 액션 종료 함수:
	// TargetMontage: 종료될 애니메이션 몽타주
	// IsProperlyEnded: 정상적으로 종료되었는지 여부를 판단합니다.
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	// 콤보 액션 종료를 알리는 가상 함수:
	// 파생 클래스에서 콤보 종료 후의 추가 처리가 필요할 경우 오버라이드 할 수 있습니다.
	virtual void NotifyComboActionEnd();

	// 콤보 입력 타이밍을 확인하기 위한 타이머 설정 함수.
	void SetComboCheckTimer();

	// 타이머가 만료되었을 때 콤보 상태를 확인하는 함수.
	void ComboCheck();

	// 현재 진행 중인 콤보의 횟수를 저장합니다.
	int32 CurrentCombo = 0;

	// 콤보 타이밍 체크를 위한 타이머 핸들.
	FTimerHandle ComboTimerHandle;

	// 다음 콤보 명령이 입력되었는지 여부를 판단하는 플래그.
	bool HasNextComboCommand = false;

	// =========================================================================================
	// 공격 관리 함수
	// =========================================================================================
protected:
	// 공격 판정 함수: 캐릭터의 공격이 상대에게 적중했는지 확인하는 기능을 수행합니다.
	// IEAnimationAttackInterface 또는 ACharacter의 기본 함수를 오버라이드 합니다.
	virtual void AttackHitCheck() override;

	// 피해 처리 함수: 캐릭터가 피해를 입었을 때, 피해량 계산 및 처리를 수행합니다.
	// EventInstigator: 공격을 발생시킨 컨트롤러
	// DamageCauser: 피해를 입힌 액터
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void CloseAttackHitCheck() override;

	// =========================================================================================
	// 활 관련 섹션
	// =========================================================================================
protected:
	TSubclassOf<class AEArrow> ArrowBP;

	virtual void ShootArrow() override;

	// =========================================================================================
	// Dead 애니메이션 섹션
	// =========================================================================================
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;

	// =========================================================================================
	// 스탯 섹션
	// =========================================================================================
protected:
	// Stat:
	// 캐릭터의 스탯(예: 체력, 공격력 등)을 관리하는 컴포넌트로,
	// 에디터에서 볼 수 있으나 블루프린트에서는 읽기 전용으로 설정되어 있습니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPricateAccess = "true"))
	TObjectPtr<class UECharacterStatComponent> Stat;

public:
	bool bIsStaggerInProgress = false;
	float CurrentStaggerGauge = 0.f;
};
