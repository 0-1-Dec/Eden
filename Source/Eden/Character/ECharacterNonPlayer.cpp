// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ECharacterNonPlayer.h"

#include "ECharacterPlayer.h"
#include "AI/GeneralAI/EAIGeneralController.h"
#include "Components/WidgetComponent.h"
#include "CharacterStat/ECharacterStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Item\EDroppedItem.h"

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

	AECharacterPlayer* Player = Cast<AECharacterPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player)
	{
		if (UECharacterStatComponent* StatComponent = Player->FindComponentByClass<UECharacterStatComponent>())
		{
			HandleDrop();
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


void AECharacterNonPlayer::HandleDrop()
{
	if(!DropData) return;

	// 경험치 지급
	AECharacterPlayer* Player = Cast<AECharacterPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if(Player)
	{
		if(UECharacterStatComponent* StatComponent = Player->FindComponentByClass<UECharacterStatComponent>())
		{
			StatComponent->OnExpGain.Broadcast(DropData->Exp);
			//StatComponent->OnGoldGain.Broadcast(DropData->Gold); // 나중에 골드 추가 시 사용
		}
	}

	// 아이템 드랍
	for(const FDropEntry& Entry : DropData->DropItems)
	{
		if(Entry.Item.IsNull()) continue;
		if(FMath::FRand() > Entry.DropChance) continue;

		int32 Count = FMath::RandRange(Entry.MinCount,Entry.MaxCount);
		UEItemDataAsset* ItemAsset = Entry.Item.LoadSynchronous();
		if(!ItemAsset) continue;

		FVector SpawnLoc = GetActorLocation();
		FRotator SpawnRot = FRotator::ZeroRotator;

		AEDroppedItem* DroppedItem = GetWorld()->SpawnActor<AEDroppedItem>(DroppedItemClass,SpawnLoc,SpawnRot);
		if(DroppedItem)
		{
			DroppedItem->Init(ItemAsset,Count);
		}
	}
}
