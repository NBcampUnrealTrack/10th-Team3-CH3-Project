#include "UI/UserHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"

namespace
{
	const FName IntensityParam(TEXT("Intensity"));
}

void UUserHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	HitFlashMID = HitFlashVignette->GetDynamicMaterial();
	FullnessMID = FullnessVignette->GetDynamicMaterial();
}

void UUserHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (HitFlashMID && HitFlashIntensity > 0.f)
	{
		HitFlashIntensity *= FMath::Exp(-HitFlashDecay * InDeltaTime);
		if (HitFlashIntensity < 0.01f) { HitFlashIntensity = 0.f; }
		HitFlashMID->SetScalarParameterValue(IntensityParam, HitFlashIntensity);
	}

	if (FullnessMID)
	{
		if (FullnessRatio < FullnessWarningStart)
		{
			FullnessMID->SetScalarParameterValue(IntensityParam, 0.f);
			PulsePhase = 0.f;
			return;
		}

		// 50%→0, 100%→1
		const float T = FMath::GetMappedRangeValueClamped(FVector2D(FullnessWarningStart, 1.f), FVector2D(0.f, 1.f), FullnessRatio);
		const float Rate = FMath::Lerp(PulseRateMin, PulseRateMax, T);
		PulsePhase = FMath::Fmod(PulsePhase + InDeltaTime * Rate, 1.f);

		// 심장박동: 빠르게 올라갔다 천천히 내려감
		const float Beat = FMath::Pow(1.f - PulsePhase, 3.f);
		const float Base = FMath::Lerp(0.25f, 1.f, T);
		FullnessMID->SetScalarParameterValue(IntensityParam, Base * (0.35f + 0.65f * Beat));
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

void UUserHUDWidget::SetWave(int32 Wave)
{
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
	HitFlashIntensity = 1.f;
}

void UUserHUDWidget::SetFullnessWarning(float Ratio)
{
	FullnessRatio = Ratio;
}
