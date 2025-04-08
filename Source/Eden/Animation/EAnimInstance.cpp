// 프로젝트 설정의 Description 페이지에서 저작권 정보를 작성하세요.

#include "Animation/EAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// 생성자: 기본 임계값들을 설정합니다.
UEAnimInstance::UEAnimInstance()
{
	// MovingThreshould: 캐릭터가 정지 상태로 간주되는 2D 속도 임계값 (예: 3.0 이하이면 idle)
	MovingThreshould = 3.0f;

	// JumpingThreshould: 캐릭터가 점프 중인지 판단할 Z축 속도 임계값 (예: 100.0 이상이면 점프로 간주)
	JumpingThreshould = 100.0f;

	CurrentWeaponType = EWeaponType::OneHanded;
}

// NativeInitializeAnimation: 애니메이션 인스턴스 초기화 시 호출되어 초기 설정을 수행합니다.
void UEAnimInstance::NativeInitializeAnimation()
{
	// 상위 클래스의 초기화 로직 실행
	Super::NativeInitializeAnimation();

	// 애니메이션 인스턴스의 소유자(액터)를 ACharacter 타입으로 캐스팅하여 저장합니다.
	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		// 캐릭터의 이동 컴포넌트를 가져와서 Movement 변수에 할당합니다.
		Movement = Owner->GetCharacterMovement();
	}
}

// NativeUpdateAnimation: 매 프레임 호출되어 애니메이션 상태를 업데이트합니다.
void UEAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	// 상위 클래스의 업데이트 로직 실행
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		// 현재 이동 컴포넌트의 속도를 Velocity에 저장합니다.
		Velocity = Movement->Velocity;
		// 2D 평면상의 속도 크기를 계산하여 지면 이동 속도를 업데이트합니다.
		GroundSpeed = Velocity.Size2D();
		
		// 지면 속도가 MovingThreshould보다 작으면 캐릭터를 정지(idle) 상태로 간주합니다.
		bIsIdle = GroundSpeed < MovingThreshould;

		if (Owner)
		{
			FRotator OwnerRotation = Owner->GetActorRotation();
			FRotationMatrix RotationMatrix(OwnerRotation);

			FVector ForwardDirection = RotationMatrix.GetUnitAxis(EAxis::X);
			FVector RightDirection = RotationMatrix.GetUnitAxis(EAxis::Y);

			ForwardBackward = FVector::DotProduct(Velocity, ForwardDirection);
			LeftRight = FVector::DotProduct(Velocity, RightDirection);
		}

		// 캐릭터가 공중에 떠 있는지(낙하 중인지)를 판단합니다.
		bIsFalling = Movement->IsFalling();
		// 점프 상태는 낙하 중이면서, Z축 속도가 JumpingThreshould보다 큰 경우로 판단합니다.
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}
}