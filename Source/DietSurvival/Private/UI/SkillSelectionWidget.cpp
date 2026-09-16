#include "UI/SkillSelectionWidget.h"
#include "UI/SkillCardWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

void USkillSelectionWidget::InitializeCards(const TArray<TTuple<FName, int32>>& Augments)
{
	if (!CardContainer || !CardWidgetClass)
	{
		return;
	}

	CardContainer->ClearChildren();
	ActiveCards.Reset();

	for (const auto& [Name, Level] : Augments)
	{
		USkillCardWidget* Card = CreateWidget<USkillCardWidget>(GetOwningPlayer(), CardWidgetClass);
		if (!Card) { continue; }

		Card->SetupCard(Name, Level);
		Card->OnCardClicked.AddDynamic(this, &USkillSelectionWidget::HandleCardClicked);

		UHorizontalBoxSlot* HorizonSlot = CardContainer->AddChildToHorizontalBox(Card);
		HorizonSlot->SetPadding(CardPadding);
		HorizonSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
		HorizonSlot->SetVerticalAlignment(VAlign_Center);
		ActiveCards.Add(Card);
	}
}

void USkillSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillSelectionWidget::HandleCardClicked(FName SkillFName)
{
	// 중복 클릭 방지. 모든 카드 비활성화.
	for (USkillCardWidget* Card : ActiveCards)
	{
		Card->SetIsEnabled(false);
	}

	OnSkillChosen.Broadcast(SkillFName);
}
