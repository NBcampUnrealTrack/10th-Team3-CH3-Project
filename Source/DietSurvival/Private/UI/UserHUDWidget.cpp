#include "UI/UserHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"

namespace
{
	const FName IntensityParam(TEXT("Intensity"));
	const FName PercentParam(TEXT("Percent"));
}

void UUserHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	HitFlashMID = HitFlashVignette->GetDynamicMaterial();
	ReloadMID = ReloadBar->GetDynamicMaterial();
}

void UUserHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bReloading && !GetWorld()->IsPaused())
	{
		ReloadElapsed += GetWorld()->GetDeltaSeconds();
		if (ReloadMID) {
			ReloadMID->SetScalarParameterValue(PercentParam, FMath::Clamp(ReloadElapsed / ReloadDuration, 0.f, 1.f));
		}
	}

	if (HitFlashMID && HitFlashIntensity > 0.f)
	{
		HitFlashIntensity *= FMath::Exp(-HitFlashDecay * InDeltaTime);
		if (HitFlashIntensity < 0.01f) { HitFlashIntensity = 0.f; }
		HitFlashMID->SetScalarParameterValue(IntensityParam, HitFlashIntensity);
	}
}

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

void UUserHUDWidget::SetAmmo(int32 CurrentAmmo)
{
	AmmoText->SetText(FText::AsNumber(CurrentAmmo));
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
	HitFlashIntensity = FMath::Lerp(HitFlashMinIntensity, HitFlashMaxIntensity, FullnessRatio);
}

void UUserHUDWidget::SetFullnessWarning(float Ratio)
{
	FullnessRatio = Ratio;
}

void UUserHUDWidget::StartReload()
{
	bReloading = true;
	ReloadElapsed = 0.f;

	if (ReloadMID) {
		ReloadMID->SetScalarParameterValue(PercentParam, 0.f);
	}

	ReloadBar->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUserHUDWidget::FinishReload()
{
	if (!bReloading) {
		return;
	}
	bReloading = false;
	ReloadBar->SetVisibility(ESlateVisibility::Collapsed);
}
