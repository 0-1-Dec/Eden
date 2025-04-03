// 프로젝트 설정의 Description 페이지에서 저작권 정보를 작성하세요.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EAnimInstance.generated.h"

/**
 * UEAnimInstance 클래스는 캐릭터의 애니메이션 상태를 업데이트하고 관리하는 역할을 합니다.
 * 이 클래스는 캐릭터의 이동, 점프, 정지 등 다양한 상태에 따라 애니메이션 블루프린트와 상호작용합니다.
 */
UCLASS()
class EDEN_API UEAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 생성자: 애니메이션 인스턴스의 기본 설정을 초기화합니다.
	UEAnimInstance();

protected:
	// NativeInitializeAnimation:
	// 애니메이션 인스턴스가 초기화될 때 호출되며, 초기 설정 및 캐싱 작업을 수행합니다.
	virtual void NativeInitializeAnimation() override;

	// NativeUpdateAnimation:
	// 매 프레임마다 호출되어 애니메이션 상태를 업데이트합니다.
	// DeltaSeconds는 이전 프레임과의 시간 차이를 나타냅니다.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Owner:
	// 이 애니메이션 인스턴스가 소유한 캐릭터 액터를 참조합니다.
	// 캐릭터의 상태, 위치, 방향 등의 정보를 얻기 위해 사용됩니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	// Movement:
	// 캐릭터의 이동 컴포넌트를 참조합니다.
	// 이를 통해 이동 상태, 속도, 점프 상태 등의 물리적 정보를 얻어 애니메이션 업데이트에 반영할 수 있습니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	// Velocity:
	// 캐릭터의 현재 이동 속도를 나타내는 벡터입니다.
	// 애니메이션 업데이트 시 이동 방향 및 속도 기반의 애니메이션 전환에 사용됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	// GroundSpeed:
	// 캐릭터가 지면 위에서 이동할 때의 속도 값을 나타냅니다.
	// 이동 애니메이션의 속도 조정 및 전환에 활용됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	// bIsIdle:
	// 캐릭터가 정지 상태인지 여부를 나타내는 불리언 값입니다.
	// 단일 비트 필드로 선언되어 메모리 사용을 최적화합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	// MovingThreshould:
	// 캐릭터가 이동 중이라고 판단하기 위한 속도 임계값입니다.
	// 이 값 이상일 때 캐릭터는 이동 상태로 간주되어 애니메이션 전환에 반영됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	// bIsFalling:
	// 캐릭터가 공중에 떠 있는지(낙하 중인지) 여부를 나타내는 불리언 값입니다.
	// 단일 비트 필드로 선언되어 메모리 사용을 최소화합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	// bIsJumping:
	// 캐릭터가 점프 중인지 여부를 나타내는 불리언 값입니다.
	// 단일 비트 필드로 선언되어 메모리 최적화를 돕습니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	// JumpingThreshould:
	// 캐릭터의 점프 상태를 판단하기 위한 임계값입니다.
	// 이 값은 점프 애니메이션 전환 및 관련 로직에 활용될 수 있습니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsBow : 1;

public:
	void SetBow(bool bIsTrue);
};
