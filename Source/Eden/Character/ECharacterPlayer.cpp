// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ECharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/EAnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStat/ECharacterStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/OverlapResult.h"
#include "Game/EGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Inventory/EInventoryComponent.h"
#include "Item/EArrow.h"
#include "Item/ESkillPathVFX.h"
#include "Physics/ECollision.h"
#include "UI/EHUDWidget.h"

AECharacterPlayer::AECharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 50.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_EPLAYERCAPSULE);

	OneHandL_WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("OneL_Weapon"));
	OneHandR_WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("OneR_Weapon"));
	OneHandL_WeaponMesh->SetupAttachment(GetMesh(), FName("Hand_lSocket"));
	OneHandR_WeaponMesh->SetupAttachment(GetMesh(), FName("Hand_rSocket"));
	OneHandL_WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OneHandR_WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BothHand_WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Both_Weapon"));
	BothHand_WeaponMesh->SetupAttachment(GetMesh(), FName("Hand_rSocket"));
	BothHand_WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Bow_WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bow_Weapon"));
	Bow_WeaponMesh->SetupAttachment(GetMesh(), FName("Hand_lSocket"));
	Bow_WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InventoryComponent = CreateDefaultSubobject<UEInventoryComponent>(TEXT("InventoryComponent"));

	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Eden/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Eden/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Eden/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Eden/Input/Actions/IA_Look.IA_Look'"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Eden/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputOneHandedRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Eden/Input/Actions/IA_OneHanded.IA_OneHanded'"));
	if (nullptr != InputOneHandedRef.Object)
	{
		OneHandedAction = InputOneHandedRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputBowRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Eden/Input/Actions/IA_Bow.IA_Bow'"));
	if (nullptr != InputBowRef.Object)
	{
		BowAction = InputBowRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputBothHandedRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Eden/Input/Actions/IA_BothHanded.IA_BothHanded'"));
	if (nullptr != InputBothHandedRef.Object)
	{
		BothHandedAction = InputBothHandedRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputOpenInventoryRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Eden/Input/Actions/IA_OpenInventory.IA_OpenInventory'"));
	if (nullptr != InputOpenInventoryRef.Object)
	{
		OpenInventoryAction = InputOpenInventoryRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputBowZoomRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Eden/Input/Actions/IA_BowZoom.IA_BowZoom'"));
	if(nullptr != InputBowZoomRef.Object)
	{
		BowZoomAction = InputBowZoomRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputOpenStatRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Eden/Input/Actions/IA_OpenStat.IA_OpenStat'"));
	if(nullptr != InputOpenStatRef.Object)
	{
		OpenStatAction = InputOpenStatRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputSkillRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Eden/Input/Actions/IA_Skill.IA_Skill'"));
	if(nullptr != InputSkillRef.Object)
	{
		SkillAction = InputSkillRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputDodgeRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Eden/Input/Actions/IA_Dodge.IA_Dodge'"));
	if(nullptr != InputDodgeRef.Object)
	{
		DodgeAction = InputDodgeRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputPotionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Eden/Input/Actions/IA_Potion.IA_Potion'"));
	if(nullptr != InputPotionRef.Object)
	{
		DrinkPotionAction = InputPotionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> OneHand_WeaponMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PostApocalypticMeleeWeapons/Meshes/SK_ShivGlass.SK_ShivGlass'"));
	if (OneHand_WeaponMeshRef.Object)
	{
		OneHandL_WeaponMesh->SetSkeletalMesh(OneHand_WeaponMeshRef.Object);
		OneHandR_WeaponMesh->SetSkeletalMesh(OneHand_WeaponMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Bow_WeaponMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Eden/Item/bow.bow'"));
	if (Bow_WeaponMeshRef.Object)
	{
		Bow_WeaponMesh->SetStaticMesh(Bow_WeaponMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BothHand_WeaponMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PostApocalypticMeleeWeapons/Meshes/SK_Rebar_Sledgehammer.SK_Rebar_Sledgehammer'"));
	if (BothHand_WeaponMeshRef.Object)
	{
		BothHand_WeaponMesh->SetSkeletalMesh(BothHand_WeaponMeshRef.Object);
	}

	OneHandL_WeaponMesh->SetVisibility(true);
	OneHandR_WeaponMesh->SetVisibility(true);
	Bow_WeaponMesh->SetVisibility(false);
	BothHand_WeaponMesh->SetVisibility(false);
}

void AECharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (Stat && InventoryComponent)
	{
		if (UEGameInstance* GI = GetGameInstance<UEGameInstance>())
		{
			GI->ApplyToComponents(Stat, InventoryComponent);
		}
	}

	Stat->OnExpGain.AddUObject(this, &AECharacterPlayer::ExpGain);
	Stat->PlayerStatDataTableLoading(Stat->GetCurrentLevel());

	SetWeaponData(OneHandedData);

	DefaultFOV = FollowCamera->FieldOfView;
	ZoomFOV = 30.f;
	ZoomSpeed = 10.f;
}

void AECharacterPlayer::SetDead()
{
	Super::SetDead();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		DisableInput(PlayerController);
	}
}

void AECharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float TargetFOV = bIsZoomedIn ? ZoomFOV : DefaultFOV;
	const float CurrentFOV = FollowCamera->FieldOfView;
	const float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, ZoomSpeed);

	FollowCamera->SetFieldOfView(NewFOV);
}

void AECharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AECharacterPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AECharacterPlayer::Look);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AECharacterPlayer::Dodge);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AECharacterPlayer::TryBowChargeStart);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AECharacterPlayer::TryBowChargeEnd);
	EnhancedInputComponent->BindAction(DrinkPotionAction, ETriggerEvent::Started, this, &AECharacterPlayer::DrinkPotion);
	

	// 무기 교체
	EnhancedInputComponent->BindAction(OneHandedAction, ETriggerEvent::Started, this, &AECharacterPlayer::SwapOneHanded);
	EnhancedInputComponent->BindAction(BowAction, ETriggerEvent::Started, this, &AECharacterPlayer::SwapBow);
	EnhancedInputComponent->BindAction(BothHandedAction, ETriggerEvent::Started, this, &AECharacterPlayer::SwapBothHanded);

	// 인벤토리
	EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &AECharacterPlayer::ToggleInventoryUI);

	// 활 줌
	EnhancedInputComponent->BindAction(BowZoomAction,ETriggerEvent::Started,this,&AECharacterPlayer::BowZoomIn);
	EnhancedInputComponent->BindAction(BowZoomAction,ETriggerEvent::Completed,this,&AECharacterPlayer::BowZoomOut);

	// 스탯
	EnhancedInputComponent->BindAction(OpenStatAction,ETriggerEvent::Started,this,&AECharacterPlayer::ToggleStatUI);

	// 스킬
	EnhancedInputComponent->BindAction(SkillAction,ETriggerEvent::Started,this,&AECharacterPlayer::ExecuteSkill);

	// 세팅 메뉴
	EnhancedInputComponent->BindAction(OpenSettingAction,ETriggerEvent::Started,this,&AECharacterPlayer::ToggleSettingUI);
}

void AECharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AECharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);

	if (bIsZoomedIn)
	{
		FRotator ControlRot = Controller->GetControlRotation();
		FRotator NewActorRot(0.f, ControlRot.Yaw, 0.f);
		SetActorRotation(NewActorRot);
	}
}


void AECharacterPlayer::Dodge(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(DodgeMontage);
}

void AECharacterPlayer::TryBowChargeStart()
{
	if (bIsBow && bIsZoomedIn)
	{
		bIsAttackInput = true;
		if (UEAnimInstance* AnimInstance = Cast<UEAnimInstance>(GetMesh()->GetAnimInstance()))
		{
			AnimInstance->bIsAttackInput = true;
		}
	}
	else
	{
		Attack();
	}
}

void AECharacterPlayer::TryBowChargeEnd()
{
	if (bIsBow && bIsZoomedIn)
	{
		if (UEAnimInstance* AnimInstance = Cast<UEAnimInstance>(GetMesh()->GetAnimInstance()))
		{
			AnimInstance->bIsAttackInput = false;
		}
		bIsAttackInput = false;
	}
}

void AECharacterPlayer::Attack()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FRotator CamRot;
		FVector CamLoc;
		PC->GetPlayerViewPoint(CamLoc, CamRot);
		FRotator NewYaw(0.f, CamRot.Yaw, 0.f);
		SetActorRotation(NewYaw);
	}
	
	ProcessComboCommand();
}

void AECharacterPlayer::SwapOneHanded()
{
	if (GetCharacterMovement()->IsFalling())
		return;
	
	if (UEAnimInstance* AnimInstance = Cast<UEAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->CurrentWeaponType = EWeaponType::OneHanded;
		AnimInstance->bIsZoomedIn = false;
		AnimInstance->bIsBow = false;
		AnimInstance->bIsBoth = false;
	}
	
	bIsBow = false;
	PlayWeaponSwapMontage(OneHandedData);

	OneHandL_WeaponMesh->SetVisibility(true);
	OneHandR_WeaponMesh->SetVisibility(true);
	Bow_WeaponMesh->SetVisibility(false);
	BothHand_WeaponMesh->SetVisibility(false);
}

void AECharacterPlayer::SwapBow()
{
	if (GetCharacterMovement()->IsFalling())
		return;
	
	if (UEAnimInstance* AnimInstance = Cast<UEAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->CurrentWeaponType = EWeaponType::Bow;
		AnimInstance->bIsZoomedIn = false;
		AnimInstance->bIsBow = true;
		AnimInstance->bIsBoth = false;
	}
	
	bIsBow = true;
	PlayWeaponSwapMontage(BowData);

	OneHandL_WeaponMesh->SetVisibility(false);
	OneHandR_WeaponMesh->SetVisibility(false);
	Bow_WeaponMesh->SetVisibility(true);
	BothHand_WeaponMesh->SetVisibility(false);
}

void AECharacterPlayer::SwapBothHanded()
{
	if (GetCharacterMovement()->IsFalling())
		return;
	
	if (UEAnimInstance* AnimInstance = Cast<UEAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->CurrentWeaponType = EWeaponType::BothHanded;
		AnimInstance->bIsZoomedIn = false;
		AnimInstance->bIsBow = false;
		AnimInstance->bIsBoth = true;
	}
	
	bIsBow = false;
	PlayWeaponSwapMontage(BothHandedData);
	
	OneHandL_WeaponMesh->SetVisibility(false);
	OneHandR_WeaponMesh->SetVisibility(false);
	Bow_WeaponMesh->SetVisibility(false);
	BothHand_WeaponMesh->SetVisibility(true);
}

void AECharacterPlayer::PlayWeaponSwapMontage(UEWeaponDataAsset* NewWeaponData)
{
	if (CurrentWeaponData == NewWeaponData)
	{
		return;
	}

	SetWeaponData(NewWeaponData);
	PendingWeaponData = NewWeaponData;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(NewWeaponData->EquipMontage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AECharacterPlayer::OnWeaponSwapMontageEnded);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, NewWeaponData->EquipMontage);
}

void AECharacterPlayer::OnWeaponSwapMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(PendingWeaponData)
	{
		PendingWeaponData = nullptr;
	}
}

void AECharacterPlayer::ToggleInventoryUI()
{
	if (!InventoryWidgetInstance && InventoryWidgetClass)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			InventoryWidgetInstance = CreateWidget<UEInventoryWidget>(PC, InventoryWidgetClass);

			const FVector2D InitialPos = FVector2D(1100.f, 200.f);
			const FVector2D DesiredSize = FVector2D(600.f, 650.f);

			InventoryWidgetInstance->SetAnchorsInViewport(FAnchors(0.f, 0.f));
			InventoryWidgetInstance->SetAlignmentInViewport(FVector2D(0.f, 0.f));

			InventoryWidgetInstance->SetPositionInViewport(InitialPos, true);
			
			InventoryWidgetInstance->SetDesiredSizeInViewport(DesiredSize);

			InventoryWidgetInstance->AddToViewport();
			InventoryWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (InventoryWidgetInstance)
	{
		if (bInventoryOpen)
		{
			InventoryWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
			bInventoryOpen = false;

			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = false;
		}
		else
		{
			InventoryWidgetInstance->SetVisibility(ESlateVisibility::Visible);
			bInventoryOpen = true;

			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(InventoryWidgetInstance->TakeWidget());
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;
		}
	}
}

void AECharacterPlayer::ShootArrow()
{
	FVector MuzzleLocation = GetMesh()->GetSocketLocation(TEXT("Hand_lSocket_0")) + GetActorForwardVector();
	FRotator MuzzleRotation;
	
	FVector CameraLocation;
	FRotator CameraRotation;
	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 10000.f);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params);

	FVector TargetPoint = bHit ? Hit.ImpactPoint : TraceEnd;

	FVector ArrowDirection = (TargetPoint - MuzzleLocation).GetSafeNormal();
	MuzzleRotation = ArrowDirection.Rotation();
	

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	if (AEArrow* Projectile = GetWorld()->SpawnActor<AEArrow>(ArrowBP, MuzzleLocation, MuzzleRotation, SpawnParams))
	{
		if (bIsZoomedIn)
		{
			Projectile->InitProjectile(BowData->BaseDamage, 5000.f);	
		}
		else
		{
			Projectile->InitProjectile(BowData->BaseDamage, 3000.f);
		}
	}
}

void AECharacterPlayer::BowZoomIn()
{
	if(bIsBow){
		if (UEAnimInstance* AnimInstance = Cast<UEAnimInstance>(GetMesh()->GetAnimInstance()))
		{
			AnimInstance->bIsZoomedIn = true;
		}

		if(!CrosshairWidgetInstance && CrosshairWidgetClass)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			if(PC)
			{
				CrosshairWidgetInstance = CreateWidget<UECrosshairWidget>(PC,CrosshairWidgetClass);
			}
		}
		
		if(CrosshairWidgetInstance)
		{
			CrosshairWidgetInstance->AddToViewport();
		}
		
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}
		
		bIsZoomedIn = true;
	}
}

void AECharacterPlayer::BowZoomOut()
{
	if (UEAnimInstance* AnimInstance = Cast<UEAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->bIsZoomedIn = false;
	}

	if(CrosshairWidgetInstance)
	{
		CrosshairWidgetInstance->RemoveFromParent();
	}
	
	bIsZoomedIn = false;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void AECharacterPlayer::ExecuteSkill()
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}
	
	if (!bCanUseSkill || PendingWeaponData)
	{
		return;
	}
	
	if (CurrentWeaponData == BowData)
	{
		APawn* FoundTarget = FindNearestPawnInAttackRange();
		if (!FoundTarget)
		{
			return;
		}

		NearestTargetPawn = FoundTarget;
	}

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FRotator CamRot;
		FVector CamLoc;
		PC->GetPlayerViewPoint(CamLoc, CamRot);
		FRotator NewYaw(0.f, CamRot.Yaw, 0.f);
		SetActorRotation(NewYaw);
	}
	
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(CurrentWeaponData->SkillMontage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AECharacterPlayer::SkillEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, CurrentWeaponData->SkillMontage);
	
	bCanUseSkill = false;
}

void AECharacterPlayer::SkillEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	if (CurrentWeaponData == BowData)
	{
		NearestTargetPawn = nullptr;
	}
	
	HUDWidgetInstance->bIsCooldownActive = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorld()->GetTimerManager().SetTimer(SkillCooldownTimerHandle, this, &AECharacterPlayer::ResetSkillCooldown, 5.f, false);
}

void AECharacterPlayer::ResetSkillCooldown()
{
	bCanUseSkill = true;
	HUDWidgetInstance->ResetCooldown();
}

APawn* AECharacterPlayer::FindNearestPawnInAttackRange()
{
	APawn* ControllingPawn = Cast<APawn>(GetController()->GetPawn());
	if (!ControllingPawn)
	{
		return nullptr;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (!World)
	{
		return nullptr;
	}
	
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_EACTION,
		FCollisionShape::MakeSphere(2000.f),
		CollisionQueryParam
	);

	DrawDebugSphere(World, Center, 2000.f, 16, FColor::Green, false, 0.2f);

	APawn* NearestPawn = nullptr;
	
	if (bResult)
	{
		float NearestDistanceSq = FLT_MAX;
		for (const FOverlapResult& Result : OverlapResults)
		{
			APawn* TestPawn = Cast<APawn>(Result.GetActor());
			if (TestPawn && TestPawn != ControllingPawn)
			{
				float DistSq = FVector::DistSquared(Center, TestPawn->GetActorLocation());
				if (DistSq < NearestDistanceSq)
				{
					NearestDistanceSq = DistSq;
					NearestPawn = TestPawn;
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Test Pawn Error"));
			}
		}
	}
	else
		UE_LOG(LogTemp, Error, TEXT("No bResult Found"));
	
	return NearestPawn;
}

void AECharacterPlayer::ShootHomingArrow(APawn* Nearest)
{
	if (Nearest)
	{
		FVector MuzzleLocation = GetMesh()->GetSocketLocation(TEXT("Hand_lSocket_0"));
		const float SpreadAngle = 30.f;

		// 반복문을 통해 3개의 화살을 발사 (중앙, 왼쪽, 오른쪽)
		for (int i = -1; i <= 1; i++)
		{
			FRotator MuzzleRotation = GetActorForwardVector().Rotation();
			MuzzleRotation.Yaw += i * SpreadAngle;
			
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			if (AEArrow* Projectile = GetWorld()->SpawnActor<AEArrow>(ArrowBP, MuzzleLocation, MuzzleRotation, SpawnParams))
			{
				if (UProjectileMovementComponent* ProjMovement = Projectile->FindComponentByClass<UProjectileMovementComponent>())
				{
					ProjMovement->bIsHomingProjectile = true;
					ProjMovement->ProjectileGravityScale = 0.f;
					ProjMovement->HomingAccelerationMagnitude = 10000.f;
					ProjMovement->HomingTargetComponent = Nearest->GetComponentByClass<UCapsuleComponent>();
				}
				Projectile->InitProjectile(BowData->BaseDamage, 1500.f);
				
				Nearest->FindComponentByClass<UECharacterStatComponent>()->OnHpZero.AddUObject(Projectile, &AEArrow::OnTargetDestroyed);
			}
		}
	}
}

void AECharacterPlayer::ExecuteBothSkill()
{
	SpawnSequentialVFX();
}

void AECharacterPlayer::SpawnSequentialVFX()
{
	FVector StartLocation = GetMesh()->GetComponentLocation() + GetActorForwardVector() * 100.f;
	FRotator ForwardVector = GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	GetWorld()->SpawnActor<AESkillPathVFX>(SkillPathVFXActor, StartLocation, ForwardVector, SpawnParams);
}

void AECharacterPlayer::ExpGain(float InExp)
{
	Stat->AddExp(InExp);
}

void AECharacterPlayer::SetupHUDWidget(class UEHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		HUDWidgetInstance = InHUDWidget;
		
		InHUDWidget->BindStatComponent(Stat);
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp());
		InHUDWidget->UpdateExpBar(Stat->GetCurrentExp());
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UEHUDWidget::UpdateHpBar);
		Stat->OnExpChanged.AddUObject(InHUDWidget, &UEHUDWidget::UpdateExpBar);
		Stat->OnLevelChanged.AddUObject(InHUDWidget, &UEHUDWidget::UpdateLevel);
		OnWeaponDataChanged.AddDynamic(InHUDWidget, &UEHUDWidget::UpdateSkillIcon);
	}
}

void AECharacterPlayer::ToggleStatUI()
{
	if(!StatPanelWidgetInstance && StatPanelWidgetClass)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if(PC)
		{
			StatPanelWidgetInstance = CreateWidget<UEStatPanelWidget>(PC,StatPanelWidgetClass);
		}

		StatPanelWidgetInstance->BindStatComponent(Stat.Get());
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if(StatPanelWidgetInstance)
	{
		if(bStatPanelOpen)
		{
			StatPanelWidgetInstance->RemoveFromParent();
			bStatPanelOpen = false;

			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = false;
		} else
		{
			StatPanelWidgetInstance->AddToViewport();
			bStatPanelOpen = true;

			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(StatPanelWidgetInstance->TakeWidget());
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;
		}
	}
}

void AECharacterPlayer::ToggleSettingUI(){
	APlayerController* PC = Cast<APlayerController>(GetController());
	if(!PC) return;

	if(!SettingUIInstance && SettingUIClass)
	{
		SettingUIInstance = CreateWidget<UUserWidget>(PC,SettingUIClass);
	}

	if(!SettingUIInstance) return;

	if(bSettingUIOpen)
	{
		SettingUIInstance->RemoveFromParent();
		bSettingUIOpen = false;

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	} else
	{
		SettingUIInstance->AddToViewport();
		bSettingUIOpen = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(SettingUIInstance->TakeWidget());
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
}

void AECharacterPlayer::DrinkPotion()
{
	Stat->HealUp(50.f);
	/*
	 * 포션 수량 부분 추가 예정
	 */
}
