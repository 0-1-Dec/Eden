// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ECharacterNonPlayer.h"

#include "ECharacterPlayer.h"
#include "AI/GeneralAI/EAIGeneralController.h"
#include "UI/EWidgetComponent.h"
#include "CharacterStat/ECharacterStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Item\EDroppedItem.h"
#include "Physics/ECollision.h"
#include "UI/EHpBarWidget.h"

AECharacterNonPlayer::AECharacterNonPlayer()
{
	AIControllerClass = AEAIGeneralController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ENPCCAPSULE);

	static ConstructorHelpers::FObjectFinder<UENPCMeshDataAsset> NPCDataAssetRef(TEXT("/Script/Eden.ENPCMeshDataAsset'/Game/Eden/GameData/NCPCharacterData/DA_NPCCharData.DA_NPCCharData'"));
	if (NPCDataAssetRef.Object)
	{
		NPCDataAsset = NPCDataAssetRef.Object;
	}

	HpBar = CreateDefaultSubobject<UEWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0, 0, 200.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/Eden/UI/WBP_HpBarWidget.WBP_HpBarWidget_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::World);
		HpBar->SetDrawSize(FVector2D(100.f, 10.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	GetMesh()->SetHiddenInGame(true);

	PrimaryActorTick.bCanEverTick = true;
}

void AECharacterNonPlayer::BeginPlay()
{
	Super::BeginPlay();

	Stat->SetMaxHp(100.f);
}

void AECharacterNonPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HpBar)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC && PC->PlayerCameraManager)
		{
			FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
			FVector WidgetLocation = HpBar->GetComponentLocation();
			FRotator NewRotation = (CameraLocation - WidgetLocation).Rotation();
			HpBar->SetWorldRotation(NewRotation);
		}
	}
}

void AECharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!NPCDataAsset.IsNull())
	{
		FSoftObjectPath DataPath = NPCDataAsset.ToSoftObjectPath();
		NPCDataHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(DataPath, FStreamableDelegate::CreateUObject(this, &AECharacterNonPlayer::OnNPCDataLoaded));
	}
	
	Stat->SetMaxHp(100.f);
}

void AECharacterNonPlayer::SetDead()
{
	Super::SetDead();


	if (AEAIGeneralController* AEIController = Cast<AEAIGeneralController>(GetController()))
	{
		AEIController->StopGeneralAI();
	}

	HandleDrop();

	HpBar->SetHiddenInGame(true);

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void AECharacterNonPlayer::OnNPCDataLoaded()
{
	UENPCMeshDataAsset* Data = NPCDataAsset.Get();
	if (Data && Data->NPCEntries.Num() > 0)
	{
		SelectedEntryIndex = FMath::RandRange(0, Data->NPCEntries.Num() - 1);
		const FNPCMeshMaterialSet& Entry = Data->NPCEntries[SelectedEntryIndex];

		if (Entry.AnimBlueprint.ToSoftObjectPath().IsValid())
		{
			AnimBPHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(Entry.AnimBlueprint.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &AECharacterNonPlayer::OnAnimBpLoaded));
		}

		if (Entry.AnimBlueprint.ToSoftObjectPath().IsValid())
		{
			WeaponDataHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(Entry.WeaponDataAsset.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &AECharacterNonPlayer::OnWeaponDataLoaded));
		}

		if (Entry.SkeletalMesh.ToSoftObjectPath().IsValid())
		{
			MeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(Entry.SkeletalMesh.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &AECharacterNonPlayer::OnMeshLoaded));
		}

		if (Entry.MaterialPaths.Num() > 0)
		{
			MaterialHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(Entry.MaterialPaths, FStreamableDelegate::CreateUObject(this, &AECharacterNonPlayer::OnMaterialsLoaded));
		}
	}
}

void AECharacterNonPlayer::OnAnimBpLoaded()
{
	if (AnimBPHandle.IsValid())
	{
		if (UAnimBlueprint* AnimBP = Cast<UAnimBlueprint>(AnimBPHandle->GetLoadedAsset()))
		{
			GetMesh()->SetAnimInstanceClass(AnimBP->GeneratedClass);
		}
	}
}

void AECharacterNonPlayer::OnWeaponDataLoaded()
{
	if (WeaponDataHandle.IsValid())
	{
		if(UEWeaponDataAsset* WeaponData = Cast<UEWeaponDataAsset>(WeaponDataHandle->GetLoadedAsset()))
		{
			CurrentWeaponData = WeaponData;
		}
	}
}

void AECharacterNonPlayer::OnMeshLoaded()
{
	if (MeshHandle.IsValid())
	{
		if (USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(MeshHandle->GetLoadedAsset()))
		{
			GetMesh()->SetSkeletalMesh(NPCMesh);
			GetMesh()->SetHiddenInGame(false);
			
			const FBoxSphereBounds LocalBounds = NPCMesh->GetBounds();
			const float Radius      = FMath::Max(LocalBounds.BoxExtent.X, LocalBounds.BoxExtent.Y);
			const float HalfHeight  = LocalBounds.BoxExtent.Z;
			
			UCapsuleComponent* Capsule = GetCapsuleComponent();
			const float OldHalfHeight = Capsule->GetUnscaledCapsuleHalfHeight();
			
			Capsule->SetCapsuleSize(Radius, HalfHeight);
			
			const float DeltaHalf = HalfHeight - OldHalfHeight;
			if (!FMath::IsNearlyZero(DeltaHalf))
			{
				AddActorWorldOffset(FVector(0, 0, DeltaHalf));
			}
			
			const float PivotOffsetZ = -LocalBounds.Origin.Z;
			GetMesh()->SetRelativeLocation(FVector(0, 0, PivotOffsetZ - 25.f));

			HpBar->SetRelativeLocation(FVector(0, 0, HalfHeight * 2));
		}
	}
}

void AECharacterNonPlayer::OnMaterialsLoaded()
{
	if (MaterialHandle.IsValid())
	{
		TArray<UObject*> LoadedAssets;
		MaterialHandle->GetLoadedAssets(LoadedAssets);

		for (int32 Slot = 0; Slot < LoadedAssets.Num(); ++Slot)
		{
			if (UMaterialInterface* Mat = Cast<UMaterialInterface>(LoadedAssets[Slot]))
			{
				GetMesh()->SetMaterial(Slot, Mat);
			}
		}
	}
}

float AECharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float AECharacterNonPlayer::GetAIDetectRange()
{
	return 600.0f;
}

float AECharacterNonPlayer::GetAIAttackRange()
{
	return 300.0f;
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

void AECharacterNonPlayer::HandleDrop()
{
	if(!DropData) return;

	// 경험치 지급
	if(AECharacterPlayer* Player = Cast<AECharacterPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(),0)))
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

		FVector SpawnLoc = GetActorLocation() - 20.f;
		FRotator SpawnRot = FRotator::ZeroRotator;

		for (int32 i = 0; i < Count; i++)
		{
			AEDroppedItem* DroppedItem = GetWorld()->SpawnActor<AEDroppedItem>(DroppedItemClass,SpawnLoc,SpawnRot);
			DroppedItem->Init(ItemAsset);
		}
	}
}

void AECharacterNonPlayer::SetUpCharacterWidget(class UEUserWidget* InUserWidget)
{
	if (UEHpBarWidget* HpBarWidget = Cast<UEHpBarWidget>(InUserWidget))
	{
		HpBarWidget->BindStatComponent(Stat);
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UEHpBarWidget::UpdateHpBar);
	}
}
