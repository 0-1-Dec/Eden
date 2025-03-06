#include "CharacterStat/ECharacterStatComponent.h"
#include "ECharacterStatComponent.h"

// ������: ������Ʈ�� �⺻ �Ӽ��� �����մϴ�.
UECharacterStatComponent::UECharacterStatComponent()
{
	// ���� ��Ÿ��� 50.0���� �ʱ�ȭ�մϴ�.
	AttackRadius = 50.0f;

	// ������Ʈ�� ���� ���� �� �ڵ����� InitializeComponent() �Լ��� ȣ���ϵ��� �����մϴ�.
	bWantsInitializeComponent = true;
}

// InitializeComponent: ������Ʈ�� �ʱ�ȭ�� �� ȣ��Ǹ�, �ʱ�ȭ �۾��� �����մϴ�.
void UECharacterStatComponent::InitializeComponent()
{
	// ���� Ŭ������ �ʱ�ȭ �Լ��� ���� ȣ���մϴ�.
	Super::InitializeComponent();

	// HP�� �ʱⰪ�� 200���� �����մϴ�.
	SetHp(200.f);
}

// ApplyDamage: �ܺο��� �������� ������ �� ȣ��Ǵ� �Լ��Դϴ�.
// InDamge: ������ ������ ��.
// ��ȯ��: ���� ����� ������ ��.
float UECharacterStatComponent::ApplyDamage(float InDamge)
{
	// ���� HP ���� �ӽ÷� �����մϴ�.
	const float PrevHp = CurrentHp;

	// ���� ������(InDamge)�� 0 �̻� InDamge ���Ϸ� �����Ͽ�, ���� ������ ����.
	const float ActualDamage = FMath::Clamp<float>(InDamge, 0, InDamge);

	// ���� HP���� ���� �������� �����Ͽ� ���ο� HP ���� �����մϴ�.
	SetHp(PrevHp - ActualDamage);

	// HP�� ���� 0 ������ ���(�ؼҷ� ����) OnHpZero �̺�Ʈ�� �߻����� ��� �� �ļ� ó���� �˸��ϴ�.
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	// ���� ����� ������ ���� ��ȯ�մϴ�.
	return ActualDamage;
}

// SetHp: ĳ������ HP ���� �����ϴ� ���� �Լ��Դϴ�.
// NewHp: ������ �� HP ��.
void UECharacterStatComponent::SetHp(float NewHp)
{
	// NewHp ���� 0�� �ִ� HP ��(200) ���̷� �����Ͽ� CurrentHp�� �����մϴ�.
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, 200.f);

	// HP�� ����Ǿ����� �˸��� ���� OnHpChanged �̺�Ʈ�� �߻���ŵ�ϴ�.
	OnHpChanged.Broadcast(CurrentHp);
}
