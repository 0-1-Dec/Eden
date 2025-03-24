// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ECharacterNonPlayer.h"
#include "AI/GeneralAI/EAIGeneralController.h"
#include "Components/WidgetComponent.h"
#include "CharacterStat/ECharacterStatComponent.h"

AECharacterNonPlayer::AECharacterNonPlayer()
{
	AIControllerClass = AEAIGeneralController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetRelativeLocation(FVector(0.0f,0.0f,200.0f));
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/Eden/UI/WBP_EnemyHpBar.WBP_EnemyHpBar_C"));
	if(UI_HUD.Succeeded())
	{
		HealthBarWidget->SetWidgetClass(UI_HUD.Class);
		HealthBarWidget->SetDrawSize(FVector2D(150.0f,50.0f));
	} else
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


void AECharacterNonPlayer::SetDead()
{
	Super::SetDead();

	//경험치 테스트 부분 (추후 변경!)
	if(GEngine)
	{
		for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if(APlayerController* PC = It->Get())
			{
				if(APawn* Pawn = PC->GetPawn())
				{
					if(UECharacterStatComponent* TempStat = Pawn->FindComponentByClass<UECharacterStatComponent>())
					{
						TempStat->AddExp(200);
					}
				}
			}
		}
	}
	//테스트 끝

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
	ProcessComboCommand();
}

void AECharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound();
}
