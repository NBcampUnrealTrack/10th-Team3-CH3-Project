#include "Augment/AugmentCardWidget.h"
#include "System/DataTableSubsystem.h"
#include "Components/Button.h"

void UAugmentCardWidget::SetupCard(FName InAugmentFName, int32 InAugmentLevel)
{
	AugmentFName = InAugmentFName;

	if (UDataTableSubsystem* Subsystem = UDataTableSubsystem::Get(this))
	{
		float StatAmount = Subsystem->GetAugmentDelta(InAugmentFName, InAugmentLevel);
		FText Description = Subsystem->GetAugmentDescription(InAugmentFName);

		FNumberFormattingOptions NumberFormat;
		NumberFormat.SetMaximumFractionalDigits(0);
		NumberFormat.SetMinimumFractionalDigits(0);
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
