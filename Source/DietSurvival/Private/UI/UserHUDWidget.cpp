#include "UI/UserHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UUserHUDWidget::SetHealth(float CurrentHP, float MaxHP)
{
	const float Ratio = (MaxHP > 0.f)
		? FMath::Clamp(CurrentHP / MaxHP, 0.f, 1.f)
		: 0.f;

	HealthBar->SetPercent(Ratio);
}

void UUserHUDWidget::SetExp(int32 CurrentExp, int32 MaxExp)
{
	const float Ratio = (MaxExp > 0)
		? FMath::Clamp(static_cast<float>(CurrentExp) / MaxExp, 0.f, 1.f)
		: 0.f;

	ExpBar->SetPercent(Ratio);
}

void UUserHUDWidget::SetLevel(int32 Level)
{
	LevelText->SetText(FText::FromString(FString::Printf(TEXT("Lv %d"), Level)));
}

void UUserHUDWidget::SetTimer(float ElapsedTime)
{
	const int32 TotalSeconds = FMath::FloorToInt(ElapsedTime);
	TimerText->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), TotalSeconds / 60, TotalSeconds % 60)));
}

void UUserHUDWidget::SetAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo)));
}

void UUserHUDWidget::SetKillCount(int32 Count)
{
	KillCountText->SetText(FText::AsNumber(Count));
}

void UUserHUDWidget::PlayHitMarker()
{
	PlayAnimation(HitMarkerAnim);
}

void UUserHUDWidget::PlayKillConfirm()
{
	PlayAnimation(KillConfirmAnim);
}
