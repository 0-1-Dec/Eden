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
#include "GameFramework/CharacterMovementComponent.h"
#include "Inventory/EInventoryComponent.h"
#include "Item/EArrow.h"
#include "Physics/ECollision.h"
#include "UI/EEnemyHPBarWidget.h"
#include "UI/EHUDWidget.h"

AECharacterPlayer::AECharacterPlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

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

	Stat->OnExpGain.AddDynamic(this, &AECharacterPlayer::ExpGain);

	SetWeaponData(OneHandedData);
}

void AECharacterPlayer::SetDead()
{
	Super::SetDead();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		DisableInput(PlayerController);
	}
}

void AECharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AECharacterPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AECharacterPlayer::Look);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AECharacterPlayer::Attack);

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
	EnhancedInputComponent->BindAction(OpenStatAction,ETriggerEvent::Started,this,&AECharacterPlayer::ToggleInventoryUI);

	// 스킬
	EnhancedInputComponent->BindAction(SkillAction,ETriggerEvent::Started,this,&AECharacterPlayer::ExecuteSkill);
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

void AECharacterPlayer::Attack()
{
	FTimerHandle TransitionTimerHandle;
	if (bIsBow)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (!bIsZoomedIn)
		{
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(BowData->AttackMontage);
				GetWorld()->GetTimerManager().SetTimer(TransitionTimerHandle, this, &AECharacterPlayer::AutoTransitionToShoot, 1.4f, false);
			}	
		}
		else
		{
			AnimInstance->Montage_JumpToSection(FName("Shoot"), BowData->AttackMontage);
		}
	}
	else
	{
		ProcessComboCommand();	
	}
}

void AECharacterPlayer::SwapOneHanded()
{
	if (UEAnimInstance* AnimInstance = Cast<UEAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->CurrentWeaponType = EWeaponType::OneHanded;
		AnimInstance->bIsZoomedIn = false;
	}
	
	bIsBow = false;
	PlayWeaponSwapMontage(OneHandedData, WeaponSwapMontage_OneHanded);

	if(CrosshairWidgetInstance)
	{
		CrosshairWidgetInstance->RemoveFromViewport();
	}

	OneHandL_WeaponMesh->SetVisibility(true);
	OneHandR_WeaponMesh->SetVisibility(true);
	Bow_WeaponMesh->SetVisibility(false);
	BothHand_WeaponMesh->SetVisibility(false);
}

void AECharacterPlayer::SwapBow()
{
	if (UEAnimInstance* AnimInstance = Cast<UEAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->CurrentWeaponType = EWeaponType::Bow;
		AnimInstance->bIsZoomedIn = false;
	}
	
	bIsBow = true;
	PlayWeaponSwapMontage(BowData, WeaponSwapMontage_Bow);

	if(!CrosshairWidgetInstance && CrosshairWidgetClass)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if(PC)
		{
			CrosshairWidgetInstance = CreateWidget<UECrosshairWidget>(PC,CrosshairWidgetClass);
		}
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if(CrosshairWidgetInstance)
	{
		CrosshairWidgetInstance->AddToViewport();
	}

	OneHandL_WeaponMesh->SetVisibility(false);
	OneHandR_WeaponMesh->SetVisibility(false);
	Bow_WeaponMesh->SetVisibility(true);
	BothHand_WeaponMesh->SetVisibility(false);
}

void AECharacterPlayer::SwapBothHanded()
{
	if (UEAnimInstance* AnimInstance = Cast<UEAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->CurrentWeaponType = EWeaponType::BothHanded;
		AnimInstance->bIsZoomedIn = false;
	}
	
	bIsBow = false;
	PlayWeaponSwapMontage(BothHandedData, WeaponSwapMontage_BothHanded);

	if(CrosshairWidgetInstance)
	{
		CrosshairWidgetInstance->RemoveFromViewport();
	}
	
	OneHandL_WeaponMesh->SetVisibility(false);
	OneHandR_WeaponMesh->SetVisibility(false);
	Bow_WeaponMesh->SetVisibility(false);
	BothHand_WeaponMesh->SetVisibility(true);
}

void AECharacterPlayer::PlayWeaponSwapMontage(UEWeaponDataAsset* NewWeaponData, UAnimMontage* Montage)
{
	if (CurrentWeaponData == NewWeaponData)
	{
		return;
	}

	if (!Montage)
	{
		CurrentWeaponData = NewWeaponData;
		SetWeaponData(NewWeaponData);
		UE_LOG(LogTemp, Warning, TEXT("Changed Weapon %s"), *NewWeaponData->GetName());
	}

	UpdateHudSkillImg();

	/*PendingWeaponData = NewWeaponData;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(Montage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AECharacterPlayer::OnWeaponSwapMontageEnded);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, Montage);*/
}

void AECharacterPlayer::OnWeaponSwapMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(PendingWeaponData)
	{
		SetWeaponData(PendingWeaponData);
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
		}
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (InventoryWidgetInstance)
	{
		if (bInventoryOpen)
		{
			InventoryWidgetInstance->RemoveFromParent();
			bInventoryOpen = false;

			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = false;
		}
		else
		{
			InventoryWidgetInstance->AddToViewport();
			bInventoryOpen = true;

			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(InventoryWidgetInstance->TakeWidget());
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;
		}
	}

	InventoryComponent->DebugPrintInventory();
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
			Projectile->InitProjectile(200.f, 4000.f);	
		}
		else
		{
			Projectile->InitProjectile(200.f, 2000.f);
		}
	}
}

void AECharacterPlayer::LoopHold()
{
	if (bIsZoomedIn)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_JumpToSection(FName("Hold"), BowData->AttackMontage);
		}
	}
}

void AECharacterPlayer::DrawAgain()
{
	if (bIsZoomedIn)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// AnimInstance->Montage_JumpToSection(FName("DrawAgain"), BowData->AttackMontage);
		}
	}
}

void AECharacterPlayer::AutoTransitionToShoot()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_JumpToSection(FName("Shoot"), BowData->AttackMontage);
	}
}

void AECharacterPlayer::BowZoomIn()
{
	if(bIsBow){
		if (UEAnimInstance* AnimInstance = Cast<UEAnimInstance>(GetMesh()->GetAnimInstance()))
		{
			AnimInstance->bIsZoomedIn = true;
			// AnimInstance->Montage_Play(BowData->AttackMontage);
		}
		
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}

		FollowCamera -> SetFieldOfView(30.f);

		if (CameraBoom)
		{
			CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.f);
		}
		
		bIsZoomedIn = true;
		AttackSpeedChange(CurrentWeaponData,1);
	}
}

void AECharacterPlayer::BowZoomOut()
{
	if (UEAnimInstance* AnimInstance = Cast<UEAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->bIsZoomedIn = false;
	}
	
	bIsZoomedIn = false;
	AttackSpeedChange(CurrentWeaponData,1000);

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	
	FollowCamera -> SetFieldOfView(90.f);
}

void AECharacterPlayer::AttackSpeedChange(UEWeaponDataAsset* WeaponData, float AttackSpeed){
	WeaponData->AttackSpeed = AttackSpeed;
}

void AECharacterPlayer::ResetSkillCooldown()
{
	bCanUseSkill = true;
	HUDWidget->ResetCooldown();
}

void AECharacterPlayer::ExecuteSkill()
{
	if (!bCanUseSkill)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill on cooldown"));
		return;
	}
	
	if (CurrentWeaponData == BowData)
	{
		BowSkill();
	}
	else
	{
		if (CurrentWeaponData->SkillMontage)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(CurrentWeaponData->SkillMontage);	
		}
	}
	
	HUDWidget->bIsCooldownActive = true;
	bCanUseSkill = false;
	GetWorld()->GetTimerManager().SetTimer(SkillCooldownTimerHandle, this, &AECharacterPlayer::ResetSkillCooldown, 5.f, false);
}

void AECharacterPlayer::BowSkill()
{
	UE_LOG(LogTemp, Display, TEXT("BowSkill"));
}

void AECharacterPlayer::ExpGain(int32 InExp)
{
	Stat->AddExp(InExp);
}

void AECharacterPlayer::SetupHUDWidget(class UEHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		HUDWidget = InHUDWidget;
		
		InHUDWidget->BindStatComponent(Stat);
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UEHUDWidget::UpdateHpBar);
	}
}

void AECharacterPlayer::UpdateHudSkillImg()
{
	if (HUDWidget)
	{
		HUDWidget->UpdateSkillIcon(CurrentWeaponData->Weapon);
	}
}

