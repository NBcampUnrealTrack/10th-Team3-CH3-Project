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
		OnGameOver.Broadcast();
	}
}

void UPlayerStatComponent::UpgradeStat(EPlayerStatType StatType, float Amount)
{
	// 모든 업그레이드는 덧셈으로 통일. 감소시키고 싶으면 음수 Amount를 넘기면 됨.
	switch (StatType)
	{
	case EPlayerStatType::MoveSpeed:
		MoveSpeed += Amount;
		break;

	case EPlayerStatType::AttackPower:
		AttackPower += Amount;
		break;

	case EPlayerStatType::AttackSpeed:
		AttackSpeed += Amount;
		break;

	case EPlayerStatType::Fullness:
		AddFullness(Amount);
		break;

	case EPlayerStatType::MaxFullness:
		MaxFullness += Amount;
		// 최대치가 바뀌면 비율(UI 게이지 등)이 달라지므로 다시 알려줌(Broadcast)
		OnFullnessChanged.Broadcast(Fullness, MaxFullness);
		break;

	case EPlayerStatType::AttackRange:
		AttackRange += Amount;
		break;

	case EPlayerStatType::AttackDirection:
		SetAttackDirection(AttackDirection + FMath::RoundToInt(Amount));
		break;

	default:
		break;
	}
}
