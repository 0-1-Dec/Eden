// ������Ʈ ������ Description ���������� ���۱� ������ �ۼ��ϼ���.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EAnimInstance.generated.h"

/**
 * UEAnimInstance Ŭ������ ĳ������ �ִϸ��̼� ���¸� ������Ʈ�ϰ� �����ϴ� ������ �մϴ�.
 * �� Ŭ������ ĳ������ �̵�, ����, ���� �� �پ��� ���¿� ���� �ִϸ��̼� �������Ʈ�� ��ȣ�ۿ��մϴ�.
 */
UCLASS()
class EDEN_API UEAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// ������: �ִϸ��̼� �ν��Ͻ��� �⺻ ������ �ʱ�ȭ�մϴ�.
	UEAnimInstance();

protected:
	// NativeInitializeAnimation:
	// �ִϸ��̼� �ν��Ͻ��� �ʱ�ȭ�� �� ȣ��Ǹ�, �ʱ� ���� �� ĳ�� �۾��� �����մϴ�.
	virtual void NativeInitializeAnimation() override;

	// NativeUpdateAnimation:
	// �� �����Ӹ��� ȣ��Ǿ� �ִϸ��̼� ���¸� ������Ʈ�մϴ�.
	// DeltaSeconds�� ���� �����Ӱ��� �ð� ���̸� ��Ÿ���ϴ�.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Owner:
	// �� �ִϸ��̼� �ν��Ͻ��� ������ ĳ���� ���͸� �����մϴ�.
	// ĳ������ ����, ��ġ, ���� ���� ������ ��� ���� ���˴ϴ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	// Movement:
	// ĳ������ �̵� ������Ʈ�� �����մϴ�.
	// �̸� ���� �̵� ����, �ӵ�, ���� ���� ���� ������ ������ ��� �ִϸ��̼� ������Ʈ�� �ݿ��� �� �ֽ��ϴ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	// Velocity:
	// ĳ������ ���� �̵� �ӵ��� ��Ÿ���� �����Դϴ�.
	// �ִϸ��̼� ������Ʈ �� �̵� ���� �� �ӵ� ����� �ִϸ��̼� ��ȯ�� ���˴ϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	// GroundSpeed:
	// ĳ���Ͱ� ���� ������ �̵��� ���� �ӵ� ���� ��Ÿ���ϴ�.
	// �̵� �ִϸ��̼��� �ӵ� ���� �� ��ȯ�� Ȱ��˴ϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	// bIsIdle:
	// ĳ���Ͱ� ���� �������� ���θ� ��Ÿ���� �Ҹ��� ���Դϴ�.
	// ���� ��Ʈ �ʵ�� ����Ǿ� �޸� ����� ����ȭ�մϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	// MovingThreshould:
	// ĳ���Ͱ� �̵� ���̶�� �Ǵ��ϱ� ���� �ӵ� �Ӱ谪�Դϴ�.
	// �� �� �̻��� �� ĳ���ʹ� �̵� ���·� ���ֵǾ� �ִϸ��̼� ��ȯ�� �ݿ��˴ϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	// bIsFalling:
	// ĳ���Ͱ� ���߿� �� �ִ���(���� ������) ���θ� ��Ÿ���� �Ҹ��� ���Դϴ�.
	// ���� ��Ʈ �ʵ�� ����Ǿ� �޸� ����� �ּ�ȭ�մϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	// bIsJumping:
	// ĳ���Ͱ� ���� ������ ���θ� ��Ÿ���� �Ҹ��� ���Դϴ�.
	// ���� ��Ʈ �ʵ�� ����Ǿ� �޸� ����ȭ�� �����ϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	// JumpingThreshould:
	// ĳ������ ���� ���¸� �Ǵ��ϱ� ���� �Ӱ谪�Դϴ�.
	// �� ���� ���� �ִϸ��̼� ��ȯ �� ���� ������ Ȱ��� �� �ֽ��ϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;
};
