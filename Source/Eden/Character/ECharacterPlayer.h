// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ECharacterBase.h"
#include "InputActionValue.h"
#include "GameData/EWeaponDataAsset.h"
#include "UI/EInventoryWidget.h"
#include "UI/ECrosshairWidget.h"
#include "ECharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class EDEN_API AECharacterPlayer : public AECharacterBase
{
	GENERATED_BODY()
	
public:
	AECharacterPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void SetDead() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> OneHandedAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> BowAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> BothHandedAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> OpenInventoryAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input,Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> BowZoomAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Input,Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> OpenStatAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void Attack();

// 무기스왑 애니메이션
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponData)
	UEWeaponDataAsset* OneHandedData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponData)
	UEWeaponDataAsset* BowData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponData)
	UEWeaponDataAsset* BothHandedData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponAnimation)
	UAnimMontage* WeaponSwapMontage_OneHanded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponAnimation)
	UAnimMontage* WeaponSwapMontage_Bow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponAnimation)
	UAnimMontage* WeaponSwapMontage_BothHanded;

	UEWeaponDataAsset* PendingWeaponData;

	void SwapOneHanded();
	void SwapBow();
	void SwapBothHanded();

	void PlayWeaponSwapMontage(UEWeaponDataAsset* NewWeaponData, UAnimMontage* Montage);
	void OnWeaponSwapMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 인벤토리 섹션
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TObjectPtr<class UEInventoryComponent> InventoryComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UEInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	UEInventoryWidget* InventoryWidgetInstance;

	bool bInventoryOpen = false;

	void ToggleInventoryUI();

	// 활 섹션
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = UI)
	TSubclassOf<UECrosshairWidget> CrosshairWidgetClass;

	UPROPERTY()
	UECrosshairWidget* CrosshairWidgetInstance;

	bool bIsBow = false;

	void BowZoomIn();
	void BowZoomOut();

	void AttackSpeedChange(UEWeaponDataAsset* NewWeaponData, float AttackSpeed);

	// 경험치 섹션
protected:
	UFUNCTION()
	void ExpGain(int32 InExp);

	// UI
public:
public:
	UPROPERTY(VisibleAnywhere,Category = UI)
	class UWidgetComponent* HealthBarWidget;
};
