#include "Player/PlayerStatComponent.h"

UPlayerStatComponent::UPlayerStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// 시작 시점에 UI 등에 현재 포만감 상태를 브로드캐스트
	OnFullnessChanged.Broadcast(Fullness, MaxFullness);
}

void UPlayerStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsGameOver)
	{
		return;
	}

	if (MoveFullnessDecayPerSecond != 0.f)
	{
		if (const AActor* Owner = GetOwner())
		{
			const float CurrentSpeed = Owner->GetVelocity().Size();
			if (CurrentSpeed > MovingSpeedThreshold)
			{
				AddFullness(-MoveFullnessDecayPerSecond * DeltaTime);
			}
		}
	}
}

void UPlayerStatComponent::AddFullness(float Amount)
{
	//이미 게임오버	상태라면 더 이상 포만감 증가 X
	if (bIsGameOver)
	{
		return;
	}

	const float PreviousFullness = Fullness;
	Fullness = FMath::Clamp(Fullness + Amount, 0.f, MaxFullness);

	//포만감 변화 Broadcast
	if (!FMath::IsNearlyEqual(PreviousFullness, Fullness))
	{
		OnFullnessChanged.Broadcast(Fullness, MaxFullness);
	}

	//현재 포만감이 최대치 넘기면 게임오버, Broadcast
	if (Fullness >= MaxFullness)
	{
		bIsGameOver = true;
		OnFullnessMax.Broadcast();
	}
}

void UPlayerStatComponent::UpgradeStat(EPlayerStatType StatType, float Amount)
{
	// 모든 업그레이드는 덧셈으로 통일. 감소시키고 싶으면 음수 Amount를 넘기면 됨.
	switch (StatType)
	{
	case EPlayerStatType::MoveSpeed:
		MoveSpeed += Amount;
		OnMoveSpeedChanged.Broadcast(GetEffectiveMoveSpeed());
		break;

	case EPlayerStatType::AttackPower:
		AttackPower += Amount;
		break;

	case EPlayerStatType::AttackSpeed:
		AttackSpeed += Amount;
		break;

	case EPlayerStatType::AttackRange:
		AttackRange += Amount;
		break;

	case EPlayerStatType::AttackDirection:
		SetAttackDirection(AttackDirection + FMath::RoundToInt(Amount));
		break;

	case EPlayerStatType::MoveFullnessDecayPerSecond:
		MoveFullnessDecayPerSecond += Amount;
		break;

	case EPlayerStatType::MaxAmmo:
		MaxAmmo += FMath::RoundToInt(Amount);
		OnMaxAmmoChanged.Broadcast(MaxAmmo);
		break;

	case EPlayerStatType::Fullness:
		AddFullness(Amount);
		break;

	case EPlayerStatType::MaxFullness:
		MaxFullness += Amount;
		// 최대치가 바뀌면 비율(UI 게이지 등)이 달라지므로 다시 알려줌(Broadcast)
		OnFullnessChanged.Broadcast(Fullness, MaxFullness);
		break;

	default:
		break;
	}
}

float UPlayerStatComponent::GetEffectiveMoveSpeed() const
{
	const float Ratio = GetFullnessRatio();

	float StrongestMultiplier = 1.f; // 아무 임계값도 안 넘었으면 배율 1.0(정상 속도)
	float HighestMatchedRatio = -1.f;

	for (const FMoveSpeedPenaltyThreshold& Penalty : MoveSpeedPenalties)
	{
		// 다음 기준임계값을 넘었을 때 업데이트. (가장 높은 기준값으로 적용하기 위해)
		if (Ratio >= Penalty.FullnessRatio && Penalty.FullnessRatio > HighestMatchedRatio)
		{
			HighestMatchedRatio = Penalty.FullnessRatio;
			StrongestMultiplier = Penalty.SpeedMultiplier;
		}
	}

	return MoveSpeed * StrongestMultiplier;
}
