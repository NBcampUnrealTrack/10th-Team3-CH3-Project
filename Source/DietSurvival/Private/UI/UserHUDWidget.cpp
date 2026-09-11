#include "UI/UserHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void UUserHUDWidget::SetFullness(float CurrentFullness, float MaxFullness)
{
	const float Ratio = (MaxFullness > 0.f)
		? FMath::Clamp(CurrentFullness / MaxFullness, 0.f, 1.f)
		: 0.f;

	FullnessBar->SetPercent(Ratio);
}

void UUserHUDWidget::SetExp(float CurrentExp, float MaxExp)
{
	const float Ratio = (MaxExp > 0)
		? FMath::Clamp(CurrentExp / MaxExp, 0.f, 1.f)
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

void UUserHUDWidget::SetWave(int32 Wave)
{
	if (!WaveText) { return; }
	WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave %d"), Wave)));
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


void UUserHUDWidget::PlayHitFlash()
{
	PlayAnimation(HitFlashAnim);
}

void UUserHUDWidget::SetFullnessWarning(bool bShow)
{
	FullnessWarningBorder->SetVisibility(bShow ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}
