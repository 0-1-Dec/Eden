// ������Ʈ ������ Description ���������� ���۱� ������ �ۼ��ϼ���.

#include "Animation/EAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// ������: �⺻ �Ӱ谪���� �����մϴ�.
UEAnimInstance::UEAnimInstance()
{
	// MovingThreshould: ĳ���Ͱ� ���� ���·� ���ֵǴ� 2D �ӵ� �Ӱ谪 (��: 3.0 �����̸� idle)
	MovingThreshould = 3.0f;

	// JumpingThreshould: ĳ���Ͱ� ���� ������ �Ǵ��� Z�� �ӵ� �Ӱ谪 (��: 100.0 �̻��̸� ������ ����)
	JumpingThreshould = 100.0f;
}

// NativeInitializeAnimation: �ִϸ��̼� �ν��Ͻ� �ʱ�ȭ �� ȣ��Ǿ� �ʱ� ������ �����մϴ�.
void UEAnimInstance::NativeInitializeAnimation()
{
	// ���� Ŭ������ �ʱ�ȭ ���� ����
	Super::NativeInitializeAnimation();

	// �ִϸ��̼� �ν��Ͻ��� ������(����)�� ACharacter Ÿ������ ĳ�����Ͽ� �����մϴ�.
	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		// ĳ������ �̵� ������Ʈ�� �����ͼ� Movement ������ �Ҵ��մϴ�.
		Movement = Owner->GetCharacterMovement();
	}
}

// NativeUpdateAnimation: �� ������ ȣ��Ǿ� �ִϸ��̼� ���¸� ������Ʈ�մϴ�.
void UEAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	// ���� Ŭ������ ������Ʈ ���� ����
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		// ���� �̵� ������Ʈ�� �ӵ��� Velocity�� �����մϴ�.
		Velocity = Movement->Velocity;
		// 2D ������ �ӵ� ũ�⸦ ����Ͽ� ���� �̵� �ӵ��� ������Ʈ�մϴ�.
		GroundSpeed = Velocity.Size2D();
		// ���� �ӵ��� MovingThreshould���� ������ ĳ���͸� ����(idle) ���·� �����մϴ�.
		bIsIdle = GroundSpeed < MovingThreshould;
		// ĳ���Ͱ� ���߿� �� �ִ���(���� ������)�� �Ǵ��մϴ�.
		bIsFalling = Movement->IsFalling();
		// ĳ���Ͱ� ���� ������ ���θ� �����մϴ�.
		// ���� ���´� ���� ���̸鼭, Z�� �ӵ��� JumpingThreshould���� ū ���� �Ǵ��մϴ�.
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}
}
