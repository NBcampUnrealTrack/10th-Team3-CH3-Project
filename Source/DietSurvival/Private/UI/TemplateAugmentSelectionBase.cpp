#include "UI/TemplateAugmentSelectionBase.h"
#include "UI/TemplateAugmentCardBase.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Button.h"

void UTemplateAugmentSelectionBase::InitializeCards(const TArray<TTuple<FName, int32>>& Augments)
{
	if (!CardContainer || !CardWidgetClass)
	{
		return;
	}

	ClearContainer();

	for (const auto& [Name, Level] : Augments)
	{
		UTemplateAugmentCardBase* Card = CreateWidget<UTemplateAugmentCardBase>(GetOwningPlayer(), CardWidgetClass);
		if (!Card) { continue; }

		Card->SetupCard(Name, Level);
		Card->OnCardClicked.AddDynamic(this, &UTemplateAugmentSelectionBase::HandleCardClicked);

		UHorizontalBoxSlot* HorizonSlot = CardContainer->AddChildToHorizontalBox(Card);
		HorizonSlot->SetPadding(CardPadding);
		HorizonSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
		HorizonSlot->SetVerticalAlignment(VAlign_Center);
		ActiveCards.Add(Card);
	}
}

void UTemplateAugmentSelectionBase::ClearContainer()
{
	CardContainer->ClearChildren();
	ActiveCards.Reset();
}

void UTemplateAugmentSelectionBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTemplateAugmentSelectionBase::HandleCardClicked(FName AugmentFName)
{
	// 중복 클릭 방지. 모든 카드 비활성화.
	for (UTemplateAugmentCardBase* Card : ActiveCards)
	{
		Card->SetIsEnabled(false);
	}

	OnAugmentChosen.Broadcast(AugmentFName);
}
