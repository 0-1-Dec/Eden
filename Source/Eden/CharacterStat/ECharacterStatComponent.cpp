#include "CharacterStat/ECharacterStatComponent.h"
#include "GameData/StatDataRow.h"

// 생성자: 컴포넌트의 기본 속성을 설정합니다.
UECharacterStatComponent::UECharacterStatComponent()
{
	// 공격 사거리를 50.0으로 초기화합니다.
	AttackRadius = 50.0f;

	// 컴포넌트가 게임 시작 시 자동으로 InitializeComponent() 함수를 호출하도록 설정합니다.
	bWantsInitializeComponent = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Stat(TEXT("/Script/Engine.DataTable'/Game/Eden/GameData/StatData/StatDataFile.StatDataFile'"));

	if(DT_Stat.Succeeded())
	{
		StatDataTable = DT_Stat.Object;
	}
}

// InitializeComponent: 컴포넌트가 초기화될 때 호출되며, 초기화 작업을 수행합니다.
void UECharacterStatComponent::InitializeComponent()
{
	// 상위 클래스의 초기화 함수를 먼저 호출합니다.
	Super::InitializeComponent();

	// HP를 초기값인 200으로 설정합니다.
	CurrentHp = GetMaxHp();
}

// ApplyDamage: 외부에서 데미지를 적용할 때 호출되는 함수입니다.
// InDamage: 적용할 데미지 값.
// 반환값: 실제 적용된 데미지 값.
float UECharacterStatComponent::ApplyDamage(float InDamage)
{
	// 현재 HP 값을 임시로 저장합니다.
	const float PrevHp = CurrentHp;

	// 들어온 데미지(InDamage)를 0 이상 InDamage 이하로 제한하여, 음수 데미지 방지.
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	// 현재 HP에서 실제 데미지를 차감하여 새로운 HP 값을 설정합니다.
	SetHp(PrevHp - ActualDamage);

	// HP가 거의 0 이하일 경우(극소량 이하) OnHpZero 이벤트를 발생시켜 사망 등 후속 처리를 알립니다.
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	// 실제 적용된 데미지 값을 반환합니다.
	return ActualDamage;
}

// SetHp: 캐릭터의 HP 값을 설정하는 내부 함수입니다.
// NewHp: 설정할 새 HP 값.
void UECharacterStatComponent::SetHp(float NewHp)
{
	// NewHp 값을 0과 최대 HP 값(200) 사이로 제한하여 CurrentHp에 저장합니다.
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, 200.f);

	// HP가 변경되었음을 알리기 위해 OnHpChanged 이벤트를 발생시킵니다.
	OnHpChanged.Broadcast(CurrentHp);
}

void UECharacterStatComponent::AddExp(int32 InExp)
{
	CurrentExp += InExp;

	CheckLevelUp();
}

void UECharacterStatComponent::DistributeStatPoint(FName StatName, float Amount)
{
	if (RemainStatPoint <= 0 || Amount <= 0)
	{
		return;
	}

	float UsedPoint = FMath::Clamp<float>(Amount, 0, static_cast<float>(RemainStatPoint));

	if (StatName == "Hp")
	{
		BonusMaxHp += UsedPoint;
	}
	else if (StatName == "Attack")
	{
		BonusAttack += UsedPoint;
	}
	else if (StatName == "Defense")
	{
		BonusDefense += UsedPoint;
	}

	RemainStatPoint -= static_cast<int32>(UsedPoint);

	CurrentHp = FMath::Min(CurrentHp, GetMaxHp());
	OnHpChanged.Broadcast(CurrentHp);
}

void UECharacterStatComponent::CheckLevelUp()
{
	if (CurrentExp >= GetStatRow(CurrentLevel)->ExpToNextLevel)
	{
		CurrentExp -= GetStatRow(CurrentLevel)->ExpToNextLevel;
		LevelUp();
	}
}

void UECharacterStatComponent::LevelUp()
{
	CurrentLevel += 1;

	RemainStatPoint += StatPointPerLevel;

	UE_LOG(LogTemp,Warning,TEXT("LEVEL UP!!"));
}

const FStatDataRow* UECharacterStatComponent::GetStatRow(int32 Level) const  //데이터 테이블 불러오는 함수!!
{
	if(!StatDataTable) return nullptr;

	FName RowName = FName(*FString::FromInt(Level));
	return StatDataTable->FindRow<FStatDataRow>(RowName,TEXT(""));
}