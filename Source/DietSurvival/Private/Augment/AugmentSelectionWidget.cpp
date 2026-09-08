#include "Augment/AugmentSelectionWidget.h"
#include "Augment/AugmentCardWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

void UAugmentSelectionWidget::InitializeCards(const TArray<TTuple<FName, int32>>& Augments)
{
	CardContainer->ClearChildren();
	ActiveCards.Reset();

	for (const auto& [Name, Level] : Augments)
	{
		UAugmentCardWidget* Card = CreateWidget<UAugmentCardWidget>(GetOwningPlayer(), CardWidgetClass);
		if (!Card) { continue; }

		Card->SetupCard(Name, Level);
		Card->OnCardClicked.AddDynamic(this, &UAugmentSelectionWidget::HandleCardClicked);

		UHorizontalBoxSlot* HorizonSlot = CardContainer->AddChildToHorizontalBox(Card);
		HorizonSlot->SetPadding(FMargin(12.f, 0.f));
		HorizonSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
		HorizonSlot->SetVerticalAlignment(VAlign_Center);
		ActiveCards.Add(Card);
	}
}

void UAugmentSelectionWidget::HandleCardClicked(FName AugmentFName)
{
	// 중복 클릭 방지. 모든 카드 비활성화.
	for (UAugmentCardWidget* Card : ActiveCards)
	{
		Card->SetIsEnabled(false);
	}

	OnAugmentChosen.Broadcast(AugmentFName);
}
