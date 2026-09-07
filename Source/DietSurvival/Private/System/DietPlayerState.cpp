#include "System/DietPlayerState.h"
#include "System/AugmentManagerComponent.h"

ADietPlayerState::ADietPlayerState()
{
	AugmentManager = CreateDefaultSubobject<UAugmentManagerComponent>(TEXT("AugmentManager"));
}

void ADietPlayerState::GainExp(int32 Amount)
{
	Exp += Amount;

	// UI 경험치 부분 갱신

	// 레벨업 처리(한 번에 큰 경험치를 얻어 2레벨 이상 증가하는 경우 대비)
	while (Exp >= MaxExp)
	{
		LevelUp();
		// 레벨업 끝난 이후에 다시 한 번 UI 경험치 부분 갱신
	}
}

void ADietPlayerState::LevelUp()
{
	Exp -= MaxExp;
	Level++;
	MaxExp += 5;

	// UI 레벨 부분 갱신

	// GameMode의 OnPlyaerLevelUp() 호출
}
