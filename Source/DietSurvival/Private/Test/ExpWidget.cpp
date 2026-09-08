#include "Test/ExpWidget.h"
#include "System/DietPlayerState.h"

void UExpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// OnLevelUp, OnExpChanged delegate를 선언해놓은 ADietPlayerState로 접근
	APlayerController* PC = GetOwningPlayer();
	ADietPlayerState* PS = PC ? PC->GetPlayerState<ADietPlayerState>() : nullptr;
	if (!PS) { return; }

	CachedPS = PS;

	// ADietPlayerState의 각 delegate에 이 UI위젯의 함수를 바인딩
	PS->OnLevelUp.AddDynamic(this, &UExpWidget::HandleLevelUp);
	PS->OnExpChanged.AddDynamic(this, &UExpWidget::HandleExpChanged);

	// 최초 1회 강제 갱신
	HandleLevelUp(PS->GetCurrentLevel());
	HandleExpChanged(PS->GetCurrentExp(), PS->GetMaxExp());
}

void UExpWidget::NativeDestruct()
{
	if (CachedPS)
	{
		CachedPS->OnLevelUp.RemoveDynamic(this, &UExpWidget::HandleLevelUp);
		CachedPS->OnExpChanged.RemoveDynamic(this, &UExpWidget::HandleExpChanged);
	}
}

void UExpWidget::HandleLevelUp(int32 NewLevel)
{
	OnLevelDisplayUpdated(NewLevel);
}

void UExpWidget::HandleExpChanged(int32 CurrentExp, int32 MaxExp)
{
	const float Percent = (MaxExp > 0)
		? FMath::Clamp(static_cast<float>(CurrentExp) / MaxExp * 100, 0.f, 100.f)
		: 0.f;

	OnExpDisplayUpdated(Percent, CurrentExp, MaxExp);
}
