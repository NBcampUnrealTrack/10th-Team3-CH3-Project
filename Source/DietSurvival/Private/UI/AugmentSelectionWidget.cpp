#include "UI/AugmentSelectionWidget.h"
#include "UI/AugmentCardWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Button.h"

void UAugmentSelectionWidget::InitializeCards(const TArray<TTuple<FName, int32>>& Augments)
{
	if (!CardContainer || !CardWidgetClass)
	{
		return;
	}

	ClearContainer();

	for (const auto& [Name, Level] : Augments)
	{
		UAugmentCardWidget* Card = CreateWidget<UAugmentCardWidget>(GetOwningPlayer(), CardWidgetClass);
		if (!Card) { continue; }

		Card->SetupCard(Name, Level);
		Card->OnCardClicked.AddDynamic(this, &UAugmentSelectionWidget::HandleCardClicked);

		UHorizontalBoxSlot* HorizonSlot = CardContainer->AddChildToHorizontalBox(Card);
		HorizonSlot->SetPadding(CardPadding);
		HorizonSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
		HorizonSlot->SetVerticalAlignment(VAlign_Center);
		ActiveCards.Add(Card);
	}
}

void UAugmentSelectionWidget::ClearContainer()
{
	CardContainer->ClearChildren();
	ActiveCards.Reset();
}

void UAugmentSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RerollButton->OnClicked.AddDynamic(this, &UAugmentSelectionWidget::HandleRerollClicked);
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

void UAugmentSelectionWidget::HandleRerollClicked()
{
	// 리롤 여러 번 누르는 상황 방지.
	RerollButton->SetIsEnabled(false);
	OnRerollPressed.Broadcast();
}
