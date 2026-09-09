#include "Augment/AugmentCardWidget.h"
#include "System/DataTableSubsystem.h"
#include "Components/Button.h"

void UAugmentCardWidget::SetupCard(FName InAugmentFName, int32 InAugmentLevel)
{
	AugmentFName = InAugmentFName;

	if (UDataTableSubsystem* Subsystem = UDataTableSubsystem::Get(this))
	{
		float StatAmount = Subsystem->GetAugmentDelta(InAugmentFName, InAugmentLevel);
		StatAmount = FMath::Abs(StatAmount); // 공격속도처럼 변화량이 음수인 경우도 양수로 표시
		FText Description = Subsystem->GetAugmentDescription(InAugmentFName);

		// 소수점 출력 설정
		int32 FractionalDigit = 0;
		if (Subsystem->IsAugmentShowFractionalDigit(InAugmentFName))
		{
			FractionalDigit = 1;
		}
		FNumberFormattingOptions NumberFormat;
		NumberFormat.SetMaximumFractionalDigits(FractionalDigit);
		NumberFormat.SetMinimumFractionalDigits(FractionalDigit);
		FText Delta = FText::AsNumber(StatAmount, &NumberFormat);

		FText AugmentDescription = FText::Format(Description, Delta);
		OnCardDataReady(FText::FromName(InAugmentFName), AugmentDescription);
	}
}

void UAugmentCardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CardButton)
	{
		CardButton->OnClicked.AddDynamic(this, &UAugmentCardWidget::HandleButtonClicked);
	}
}

void UAugmentCardWidget::HandleButtonClicked()
{
	OnCardClicked.Broadcast(AugmentFName);
}
