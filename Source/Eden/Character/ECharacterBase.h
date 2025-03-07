// ������Ʈ ������ Description ���������� ���۱� ������ �ۼ��ϼ���.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/EAnimationAttackInterface.h"
#include "ECharacterBase.generated.h"

// AECharacterBase Ŭ������ ACharacter�� ��ӹ޾� ���� ĳ������ �⺻ ������ �����ϸ�,
// IEAnimationAttackInterface�� �����Ͽ� �ִϸ��̼� ���� ���� ����� �����մϴ�.
UCLASS()
class EDEN_API AECharacterBase : public ACharacter, public IEAnimationAttackInterface
{
	GENERATED_BODY()

public:
	// ������: �� ĳ���� Ŭ������ �⺻ �Ӽ� ������ �ʱ�ȭ�մϴ�.
	AECharacterBase();

	virtual void PostInitializeComponents() override;

	// =========================================================================================
	// �޺� �ִϸ��̼� ����
	// =========================================================================================
protected:
	// ComboActionMontage:
	// �������Ʈ�� �����Ϳ��� ������ �� ������, �޺� �׼ǿ� ���Ǵ� �ִϸ��̼� ��Ÿ�ָ� �����մϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	// ComboActionData:
	// �޺� ���ݰ� ���õ� �����͸� �����ϴ� ��ü��, �����Ϳ��� ���� ���������� �������Ʈ������ �б� �����Դϴ�.
	// Meta �Ӽ� AllowPrivateAccess�� ���� ���� ������ ����մϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UEComboActionData> ComboActionData;

	// �޺� ����� ó���ϴ� �Լ�: �÷��̾��� �Է¿� ���� �޺� ���� ó���� �����մϴ�.
	void ProcessComboCommand();

	// �޺� �׼� ���� �Լ�: �޺� ������ ���� �� ȣ��Ǿ� �ִϸ��̼� ���� �ʱ�ȭ�մϴ�.
	void ComboActionBegin();

	// �޺� �׼� ���� �Լ�:
	// TargetMontage: ����� �ִϸ��̼� ��Ÿ��
	// IsProperlyEnded: ���������� ����Ǿ����� ���θ� �Ǵ��մϴ�.
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	// �޺� �׼� ���Ḧ �˸��� ���� �Լ�:
	// �Ļ� Ŭ�������� �޺� ���� ���� �߰� ó���� �ʿ��� ��� �������̵� �� �� �ֽ��ϴ�.
	virtual void NotifyComboActionEnd();

	// �޺� �Է� Ÿ�̹��� Ȯ���ϱ� ���� Ÿ�̸� ���� �Լ�.
	void SetComboCheckTimer();

	// Ÿ�̸Ӱ� ����Ǿ��� �� �޺� ���¸� Ȯ���ϴ� �Լ�.
	void ComboCheck();

	// ���� ���� ���� �޺��� Ƚ���� �����մϴ�.
	int32 CurrentCombo = 0;

	// �޺� Ÿ�̹� üũ�� ���� Ÿ�̸� �ڵ�.
	FTimerHandle ComboTimerHandle;

	// ���� �޺� ����� �ԷµǾ����� ���θ� �Ǵ��ϴ� �÷���.
	bool HasNextComboCommand = false;

	// =========================================================================================
	// ���� ���� �Լ�
	// =========================================================================================
protected:
	// ���� ���� �Լ�: ĳ������ ������ ��뿡�� �����ߴ��� Ȯ���ϴ� ����� �����մϴ�.
	// IEAnimationAttackInterface �Ǵ� ACharacter�� �⺻ �Լ��� �������̵� �մϴ�.
	virtual void AttackHitCheck() override;

	// ���� ó�� �Լ�: ĳ���Ͱ� ���ظ� �Ծ��� ��, ���ط� ��� �� ó���� �����մϴ�.
	// EventInstigator: ������ �߻���Ų ��Ʈ�ѷ�
	// DamageCauser: ���ظ� ���� ����
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// =========================================================================================
	// Dead �ִϸ��̼� ����
	// =========================================================================================
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;

	// =========================================================================================
	// ���� ����
	// =========================================================================================
protected:
	// Stat:
	// ĳ������ ����(��: ü��, ���ݷ� ��)�� �����ϴ� ������Ʈ��,
	// �����Ϳ��� �� �� ������ �������Ʈ������ �б� �������� �����Ǿ� �ֽ��ϴ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPricateAccess = "true"))
	TObjectPtr<class UECharacterStatComponent> Stat;
};
