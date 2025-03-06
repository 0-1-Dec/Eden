// ������Ʈ ������ Description ���������� ���۱� ������ �ۼ��ϼ���.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EComboActionData.generated.h"

/**
 * UEComboActionData�� �޺� ���� �ִϸ��̼ǿ� �ʿ��� �����͸� �����ϴ� Data Asset�Դϴ�.
 * �� Ŭ������ �޺� �ִϸ��̼� ���� �̸��� ���λ�, �ִ� �޺� Ƚ��, �ִϸ��̼� ������ ����Ʈ,
 * �� �޺� �ܰ迡�� ��ȿ�� ������ ���� �����Ͽ� �޺� �ý����� Ÿ�̹��� �����ϴµ� ���˴ϴ�.
 */
UCLASS()
class EDEN_API UEComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// ������: �⺻ �ʱ�ȭ �۾��� �����մϴ�.
	UEComboActionData();

	// MontageSectionNamePrefix:
	// �޺� �ִϸ��̼��� �� ���� �̸��� ����� ���λ縦 �����մϴ�.
	// ���� ��� "Combo"�� �����ϸ�, �ִϸ��̼� ���� �̸��� "Combo1", "Combo2", ... �� ���� ������ �� �ֽ��ϴ�.
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	// MaxComboCount:
	// �ִ� �޺� Ƚ���� �����մϴ�.
	// ��: �ִ� 3ȸ �޺� �����ϸ� 3���� �����մϴ�.
	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

	// FrameRate:
	// �ִϸ��̼��� ������ ����Ʈ�� �����մϴ�.
	// �� ���� �޺� �Է� Ÿ�̹� ��� �� ȿ�� �ð� ������ ���˴ϴ�.
	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;

	// EffectiveFrameCount:
	// �� �޺� �ܰ躰�� ��ȿ�� ������ ���� �����ϴ� �迭�Դϴ�.
	// �� �迭�� �� ���� �ش� �޺� �ܰ迡�� �Է��� ��ȿ�� �ð���(������ ������) �����ϴ� �� ���˴ϴ�.
	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;
};
