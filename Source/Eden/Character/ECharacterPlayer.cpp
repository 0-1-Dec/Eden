// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ECharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
}

void AECharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		//Subsystem->RemoveMappingContext(DefaultMappingContext);
	}

}

void AECharacterPlayer::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
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
	EnhancedInputComponent->BindAction(OneHandedAction, ETriggerEvent::Started, this, &AECharacterPlayer::SwapOneHanded);
	EnhancedInputComponent->BindAction(BowAction, ETriggerEvent::Started, this, &AECharacterPlayer::SwapBow);
	EnhancedInputComponent->BindAction(BothHandedAction, ETriggerEvent::Started, this, &AECharacterPlayer::SwapBothHanded);
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

}

void AECharacterPlayer::Attack()
{
	ProcessComboCommand();
}

void AECharacterPlayer::SwapOneHanded()
{
	PlayWeaponSwapMontage(EWeaponType::OneHanded, WeaponSwapMontage_OneHanded);
}

void AECharacterPlayer::SwapBow()
{
	PlayWeaponSwapMontage(EWeaponType::Bow, WeaponSwapMontage_Bow);
}

void AECharacterPlayer::SwapBothHanded()
{
	PlayWeaponSwapMontage(EWeaponType::BothHanded, WeaponSwapMontage_BothHanded);
}

void AECharacterPlayer::PlayWeaponSwapMontage(EWeaponType NewWeaponType, UAnimMontage* Montage)
{
	if (CurrentWeaponType == NewWeaponType)
	{
		return;
	}

	if (!Montage)
	{
		CurrentWeaponType = NewWeaponType;
		SetWeaponType(NewWeaponType);
		UE_LOG(LogTemp, Warning, TEXT("Changed Weapon"));
		return;
	}

	/*PendingWeaponType = NewWeaponType;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(Montage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AECharacterPlayer::OnWeaponSwapMontageEnded);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, Montage);*/
}

void AECharacterPlayer::OnWeaponSwapMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	SetWeaponType(PendingWeaponType);
	/*CurrentWeaponType = PendingWeaponType;

	OnWeaponTypeChanged.Broadcast(CurrentWeaponType);*/
}
