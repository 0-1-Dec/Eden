// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ECharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "EComboActionData.h"
#include "Physics/ECollision.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/ECharacterStatComponent.h"

// Sets default values
AECharacterBase::AECharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ECAPSULE);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Eden/Animation/ABP_ECharacter.ABP_ECharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Eden/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UEComboActionData> ComboActionDataRef(TEXT("/Script/Eden.EComboActionData'/Game/Eden/ChracterAction/EA_ComboAttack.EA_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	// ���� ������Ʈ
	Stat = CreateDefaultSubobject<UECharacterStatComponent>(TEXT("Stat"));
}

// ProcessComboCommand: �޺� Ŀ�ǵ带 ó���ϴ� �Լ��Դϴ�.
// - ���� �޺��� ���� ������ �ʴٸ� �޺� �׼��� �����մϴ�.
// - Ÿ�̸Ӱ� ��ȿ���� ������ ���� �޺� ����� ������ �����ϰ�, ��ȿ�ϴٸ� �߰� �޺� �Է��� �������� ǥ���մϴ�.
void AECharacterBase::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

// ComboActionBegin: �޺� �׼��� �����ϴ� �Լ��Դϴ�.
// - �޺� ī��Ʈ�� 1�� �ʱ�ȭ�ϰ�, �̵��� �����մϴ�.
// - ������ �޺� �ִϸ��̼� ��Ÿ�ָ� ����ϰ�, ��Ÿ�� ���� ��������Ʈ�� ���ε��մϴ�.
// - ���� �޺� �Է��� üũ�ϱ� ���� Ÿ�̸Ӹ� �����մϴ�.
void AECharacterBase::ComboActionBegin()
{
	// �޺� ���� �ʱ�ȭ
	CurrentCombo = 1;

	// �̵� ����: �޺� ���� �߿��� ĳ���Ͱ� �������� �ʵ��� ����
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// �ִϸ��̼� ����: �޺� �ִϸ��̼� ��Ÿ�� ���
	const float AttackSpeedRate = 1.f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	// ��Ÿ�� ���� ��������Ʈ ����: �޺� ���� �� ȣ��� �Լ� ���ε�
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AECharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	// ���� Ÿ�̸� ��ȿȭ ��, �޺� �Է� üũ Ÿ�̸� ����
	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

// ComboActionEnd: �޺� �׼��� ����� �� ȣ��Ǵ� �Լ��Դϴ�.
// - �޺� ���� ���¸� �ʱ�ȭ�ϰ�, �̵� ��带 �ٽ� �ȱ�� ��ȯ�մϴ�.
// - �Ļ� Ŭ�������� �߰� ó���� �ʿ��� ��� NotifyComboActionEnd()�� ȣ���մϴ�.
void AECharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// �޺� ���� ���°� �ùٸ��� ������ ��� �߻���Ŵ
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;

	// �̵� ��带 �ٽ� �ȱ�� ����
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	// �޺� ���� �� �߰� ó���� ���� ���� �Լ� ȣ�� (�Ļ� Ŭ�������� �������̵� ����)
	NotifyComboActionEnd();
}

// NotifyComboActionEnd: �޺� �׼� ���� �� ȣ��Ǵ� ���� �Լ��Դϴ�.
// �Ļ� Ŭ�������� �޺� ���� �� �߰����� ������ ������ �� �ֵ��� ����Ǿ����ϴ�.
void AECharacterBase::NotifyComboActionEnd()
{
	// �⺻ ������ �� �Լ��Դϴ�.
}

// SetComboCheckTimer: �޺� �Է��� ��ȿ �ð�(Effective Time)�� ����Ͽ� Ÿ�̸Ӹ� �����մϴ�.
// - ComboActionData�� ���ǵ� ������ ���� ������ ����Ʈ�� ����Ͽ� ��ȿ �ð��� �����մϴ�.
void AECharacterBase::SetComboCheckTimer()
{
	// ���� �޺� �ε��� ��� (0���� ����)
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.f;
	// ��ȿ �ð� ���: (������ �� / ������ ����Ʈ) / ���� �ӵ�
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		// ������ �ð� �� ComboCheck() �Լ��� ȣ��ǵ��� Ÿ�̸Ӹ� �����մϴ�.
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AECharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

// ComboCheck: Ÿ�̸� ���� �� �߰� �޺� �Է� ���θ� Ȯ���ϴ� �Լ��Դϴ�.
// - �߰� �޺� �Է��� �ִٸ�, �޺� ī��Ʈ�� ������Ű�� �ִϸ��̼� ��Ÿ���� ���� �������� ��ȯ�մϴ�.
void AECharacterBase::ComboCheck()
{
	// Ÿ�̸� ��ȿȭ
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		// ���� �޺� ī��Ʈ�� 1 ������Ű�� �ִ� �޺� ������ ���� �ʵ��� �մϴ�.
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		// ���� �޺� �ִϸ��̼� ���� �̸� ���� (��: Section1, Section2, ...)
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		// ��Ÿ�� ������ ���� �������� �����Ͽ� �ִϸ��̼��� ��ȯ�մϴ�.
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		// �߰� �޺� �Է� Ȯ���� ���� Ÿ�̸� �缳��
		SetComboCheckTimer();
		// ���� �޺� ��� �÷��� �ʱ�ȭ
		HasNextComboCommand = false;
	}
}

// AttackHitCheck: ������ ��뿡�� �����ߴ��� Ȯ���ϴ� �Լ��Դϴ�.
// - Sweep ������� �ݸ��� üũ�� �����Ͽ�, ��õ� ���� ���� ��󿡰� �������� �����մϴ�.
void AECharacterBase::AttackHitCheck()
{
	// �浹 ������ ������ ����
	FHitResult OutHitResult;
	// �ݸ��� ���� �Ķ���� ���� (�ڽ��� ����)
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	// ���� ����, �ݰ�, �׸��� ������ ���� �����մϴ�.
	const float AttackRange = 40.f;
	const float AttackRadius = 50.f;
	const float AttackDamage = 50.f;

	// ���� ���� ����: ĳ���� ��ġ�� �ݸ��� ĸ�� �ݰ游ŭ ������ ��ġ
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	// ���� �� ����: ���� �������� �߰��� ������ ��ġ
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	// ������ ���� ������ ���� �浹 �˻縦 �����մϴ�.
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_EACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	// ���� ��, ��� ���Ϳ� �������� �����մϴ�.
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

	// ����� �������� ���� ������ �ð�ȭ�մϴ�.
#if ENABLE_DRAW_DEBUG

	// ����� ĸ���� �߽ɰ� �� ���̸� ����մϴ�.
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	// ���� ���ο� ���� ������ �����մϴ� (����: �ʷ�, ������: ����).
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	// ĸ�� ���·� ���� ������ ����� ������մϴ�.
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

// TakeDamage: ĳ���Ͱ� �������� ���� �� ȣ��Ǵ� �Լ��Դϴ�.
// - ���� Ŭ������ TakeDamage�� ȣ���� ��, Stat ������Ʈ�� �������� �����մϴ�.
float AECharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// ���� Ŭ������ ������ ó���� �����մϴ�.
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// ĳ������ ���� ������Ʈ�� �������� �ݿ��մϴ�.
	Stat->ApplyDamage(DamageAmount);

	// ����� ������ ���� ��ȯ�մϴ�.
	return DamageAmount;
}
