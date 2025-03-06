// ������Ʈ ������ Description ���������� ���۱� ������ �ۼ��ϼ���.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ECharacterStatComponent.generated.h"

// HP�� 0�� �Ǿ��� �� �߻��ϴ� �̺�Ʈ ��������Ʈ ����
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);

// HP�� ����� �� ȣ��Ǹ�, ����� HP ���� �����ϴ� �̺�Ʈ ��������Ʈ ����
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float)

// UECharacterStatComponent Ŭ������ ĳ������ ����(��: HP, ���� ��Ÿ� ��)�� �����ϴ� ������Ʈ�Դϴ�.
// Blueprint���� ������ �����ϵ��� meta ������ �����ϰ� �ֽ��ϴ�.
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EDEN_API UECharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// ������: ������Ʈ�� �⺻ �Ӽ� ���� �ʱ�ȭ�մϴ�.
	UECharacterStatComponent();

protected:
	// ������Ʈ�� �ʱ�ȭ�� �� ȣ��Ǵ� �Լ�.
	// �ʱ�ȭ �۾��̳� �ٸ� ������Ʈ���� ���� �۾��� ������ �� �ֽ��ϴ�.
	virtual void InitializeComponent() override;

public:
	// HP�� 0�� �Ǿ��� �� ȣ��Ǵ� ��������Ʈ (��: ĳ���� ��� ó��)
	FOnHpZeroDelegate OnHpZero;

	// HP�� ����� ������ ȣ��Ǵ� ��������Ʈ (�Ķ����: ����� HP ��)
	FOnHpChangedDelegate OnHpChanged;

	// �ζ��� �Լ�: ���� HP ���� ��ȯ�մϴ�.
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }

	// �ζ��� �Լ�: ���� ��Ÿ� ���� ��ȯ�մϴ�.
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }

	// ApplyDamage: �ܺο��� �������� ������ �� ȣ��Ǵ� �Լ�.
	// ������ ��(InDamage)�� �޾� HP�� �����ϰ�, ���� �̺�Ʈ�� �߻���ŵ�ϴ�.
	float ApplyDamage(float InDamge);

protected:
	// SetHp: HP ���� �����ϴ� ���� �Լ�.
	// HP ���� ��, OnHpChanged ��������Ʈ�� ȣ���ϰ�, HP�� 0�� �Ǹ� OnHpZero ��������Ʈ�� �����մϴ�.
	void SetHp(float NewHp);

	// ���� HP ��.
	// Transient, VisibleInstanceOnly �Ӽ��� ��Ÿ�� ���ȿ��� ��ȿ�ϸ� �����Ϳ��� �� �� �ֵ��� �մϴ�.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	// ���� ��Ÿ�: ĳ������ ���� ������ ��Ÿ���� ��.
	// Transient, VisibleInstanceOnly �Ӽ����� �����Ǿ� ��Ÿ�� ���� ���� �����˴ϴ�.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float AttackRadius;
};
