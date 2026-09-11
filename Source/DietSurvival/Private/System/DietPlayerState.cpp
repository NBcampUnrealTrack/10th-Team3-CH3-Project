#include "System/DietPlayerState.h"
#include "System/AugmentManagerComponent.h"

ADietPlayerState::ADietPlayerState()
{
	// 경험치 점진적 증가를 위한 틱 On
	PrimaryActorTick.bCanEverTick = true;

	AugmentManager = CreateDefaultSubobject<UAugmentManagerComponent>(TEXT("AugmentManager"));
}

void ADietPlayerState::GainExp(int32 Amount)
{
	if (Amount <= 0) return;

	PendingExp += (float)Amount;
}

void ADietPlayerState::ApplyExp(float Amount)
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

float ADietPlayerState::CalculateTickExp(float DeltaTime)
{
	float TickExp = ExpAbsorbRate * DeltaTime;
	if (PendingExp < TickExp) { return PendingExp; }


	return 0.0f;
}

void ADietPlayerState::BeginPlay()
{
	Super::BeginPlay();
	//GetWorldTimerManager().SetTimer(TestExpTimer, this, &ADietPlayerState::TestGainExp, 1.f, true);
}

void ADietPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 적용할 경험치가 없다면 return
	if (FMath::IsNearlyZero(PendingExp) || PendingExp <= 0.f) { return; }

	// 이번 프레임에서 적용할 경험치 계산
	float TickExp = ExpAbsorbRate * DeltaTime;
	TickExp = FMath::Min(TickExp, PendingExp);

	if (FMath::IsNearlyZero(TickExp) || TickExp <= 0) { return; }
	PendingExp -= TickExp;
	ApplyExp(TickExp);
}

void ADietPlayerState::TestGainExp()
{
	GainExp(5);
}
