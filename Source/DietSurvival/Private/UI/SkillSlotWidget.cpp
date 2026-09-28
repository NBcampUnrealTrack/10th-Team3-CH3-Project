#include "UI/SkillSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "TimerManager.h"

void USkillSlotWidget::SetSkill(UTexture2D* Icon, int32 Level, double InCooldownEndTime)
{
	IconImage->SetBrushFromTexture(Icon);
	IconImage->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (LevelText)
	{
		LevelText->SetText(FText::AsNumber(Level));
		LevelText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	CooldownEndTime = InCooldownEndTime;
	UpdateCooldown();
}

void USkillSlotWidget::ClearSkill()
{
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimer);

	IconImage->SetVisibility(ESlateVisibility::Hidden);
	CooldownText->SetVisibility(ESlateVisibility::Collapsed);
	if (LevelText)
	{
		LevelText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USkillSlotWidget::SetSelected(bool bSelected)
{
	if (SelectedFrame)
	{
		SelectedFrame->SetVisibility(bSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}

void USkillSlotWidget::UpdateCooldown()
{
	const double Remaining = CooldownEndTime - GetWorld()->GetTimeSeconds();
	if (Remaining <= 0.0)
	{
		CooldownText->SetVisibility(ESlateVisibility::Collapsed);
		IconImage->SetColorAndOpacity(FLinearColor::White);
		return;
	}

	const int32 Seconds = FMath::CeilToInt32(Remaining);
	CooldownText->SetText(FText::AsNumber(Seconds));
	CooldownText->SetVisibility(ESlateVisibility::HitTestInvisible);
	IconImage->SetColorAndOpacity(CooldownTint);

	// 표시 숫자가 바뀌는 다음 초까지만 기다림. 0초 타이머는 등록이 안 되므로 최소값 보장
	const float NextUpdate = FMath::Max(static_cast<float>(Remaining - (Seconds - 1)), UE_KINDA_SMALL_NUMBER);
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &USkillSlotWidget::UpdateCooldown, NextUpdate, false);
}

void USkillSlotWidget::NativeDestruct()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(CooldownTimer);
	}
	Super::NativeDestruct();
}
