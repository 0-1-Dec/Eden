// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ECharacterNonPlayer.h"

#include "ECharacterPlayer.h"
#include "AI/GeneralAI/EAIGeneralController.h"
#include "Components/WidgetComponent.h"
#include "CharacterStat/ECharacterStatComponent.h"
#include "Kismet/GameplayStatics.h"

AECharacterNonPlayer::AECharacterNonPlayer()
{
	AIControllerClass = AEAIGeneralController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(GetMesh());
	HealthBarWidget->SetRelativeLocation(FVector(0.0f,0.0f,200.0f));
	
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/Eden/UI/WBP_EnemyHpBar.WBP_EnemyHpBar_C"));
	if(UI_HUD.Succeeded())
	{
		HealthBarWidget->SetWidgetClass(UI_HUD.Class);
		HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
		HealthBarWidget->SetDrawSize(FVector2D(100.0f,10.0f));
		HealthBarWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("UI_HPBar 위젯 로딩 실패"));
	}
}

void AECharacterNonPlayer::BeginPlay()
{
	Super::BeginPlay();

	if(UUserWidget* Widget = HealthBarWidget->GetUserWidgetObject())
	{
		if(UEEnemyHPBarWidget* HpBar = Cast<UEEnemyHPBarWidget>(Widget))
		{
			HpBar->BindStatComponent(Stat); // ECharacterBase에서 만든 Stat
		}
	}
}

void AECharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->SetMaxHp(100.f);
}

void AECharacterNonPlayer::SetDead()
{
	Super::SetDead();


	if (AEAIGeneralController* AEIController = Cast<AEAIGeneralController>(GetController()))
	{
		AEIController->StopGeneralAI();
	}
  
	//경험치 테스트 부분 (추후 변경!)
	// if(GEngine)
	// {
	// 	for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	// 	{
	// 		if(APlayerController* PC = It->Get())
	// 		{
	// 			if(APawn* Pawn = PC->GetPawn())
	// 			{
	// 				if(UECharacterStatComponent* TempStat = Pawn->FindComponentByClass<UECharacterStatComponent>())
	// 				{
	// 					TempStat->AddExp(200);
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	//테스트 끝

	AECharacterPlayer* Player = Cast<AECharacterPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player)
	{
		if (UECharacterStatComponent* StatComponent = Player->FindComponentByClass<UECharacterStatComponent>())
		{
			StatComponent->OnExpGain.Broadcast(200.f);
		}
	}

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

float AECharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float AECharacterNonPlayer::GetAIDetectRange()
{
	return 400.0f;
}

float AECharacterNonPlayer::GetAIAttackRange()
{
	return 140.0f;
}

float AECharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void AECharacterNonPlayer::SetAIAttackDelegate(const FGeneralAIAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AECharacterNonPlayer::AttackByAI()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackByAI called"));
	ProcessComboCommand();
}

void AECharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound();
}
