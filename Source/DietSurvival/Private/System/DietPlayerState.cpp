#include "System/DietPlayerState.h"
#include "System/AugmentManagerComponent.h"

ADietPlayerState::ADietPlayerState()
{
	AugmentManager = CreateDefaultSubobject<UAugmentManagerComponent>(TEXT("AugmentManager"));
}

void ADietPlayerState::GainExp(int32 Amount)
{
	Exp += Amount;

	OnExpChanged.Broadcast(Exp, MaxExp);

	// 레벨업 처리(한 번에 큰 경험치를 얻어 2레벨 이상 증가하는 경우 대비)
	while (Exp >= MaxExp)
	{
		LevelUp();
		OnExpChanged.Broadcast(Exp, MaxExp);
	}
}

void ADietPlayerState::LevelUp()
{
	Exp -= MaxExp;
	Level++;
	MaxExp += 5;

	OnLevelUp.Broadcast(Level);
}

void ADietPlayerState::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TestExpTimer, this, &ADietPlayerState::TestGainExp, 1.f, true);
}

void ADietPlayerState::TestGainExp()
{
	GainExp(5);
}
