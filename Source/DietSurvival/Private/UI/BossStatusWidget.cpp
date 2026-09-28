#include "UI/BossStatusWidget.h"
#include "Components/ProgressBar.h"
#include "Enemy/DietBossEnemy.h"

void UBossStatusWidget::SetBoss(ADietBossEnemy* Boss)
{
	if (!Boss)
	{
		return;
	}

	CachedBoss = Boss;
	Boss->OnBossHealthChanged.AddDynamic(this, &UBossStatusWidget::HandleBossHealthChanged);
	// 체력 방송은 피격 때만 오고, 페이즈 시작은 스폰 직후라 체력이 가득 찬 상태
	HealthBar->SetPercent(1.f);
}

void UBossStatusWidget::SetRemainingTime(float RemainingSeconds)
{
	// 페이즈 시작 직후 첫 방송값이 제한 시간 (DietGameState::StartBossPhaseTimer)
	if (TimeLimit <= 0.f)
	{
		TimeLimit = RemainingSeconds;
	}
	TimeBar->SetPercent(TimeLimit > 0.f ? RemainingSeconds / TimeLimit : 0.f);
}

void UBossStatusWidget::HandleBossHealthChanged(int32 CurrentHealth, int32 MaxHealth)
{
	HealthBar->SetPercent(static_cast<float>(CurrentHealth) / MaxHealth);
}

void UBossStatusWidget::NativeDestruct()
{
	if (CachedBoss.IsValid())
	{
		CachedBoss->OnBossHealthChanged.RemoveDynamic(this, &UBossStatusWidget::HandleBossHealthChanged);
	}
	Super::NativeDestruct();
}
