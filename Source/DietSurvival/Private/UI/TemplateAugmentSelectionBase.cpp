#include "UI/TemplateAugmentSelectionBase.h"
#include "UI/TemplateAugmentCardBase.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

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
		Card->OnAppearFinished.AddDynamic(this, &UTemplateAugmentSelectionBase::HandleCardAppearFinished);
		Card->OnChosenFinished.AddDynamic(this, &UTemplateAugmentSelectionBase::HandleCardChosenFinished);

		UHorizontalBoxSlot* HorizonSlot = CardContainer->AddChildToHorizontalBox(Card);
		HorizonSlot->SetPadding(CardPadding);
		HorizonSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
		HorizonSlot->SetVerticalAlignment(VAlign_Center);
		ActiveCards.Add(Card);
	}

	// 리롤처럼 이미 떠 있는 창이면 여기서 바로. 처음 뜰 때는 NativeConstruct에서
	if (IsConstructed())
	{
		PlayAppearAll();
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
	PlayAppearAll();
}

void UTemplateAugmentSelectionBase::SetInteractionEnabled(bool bEnabled)
{
	// 비활성 대신 히트 테스트만 꺼서 카드가 회색으로 바뀌지 않게
	for (UTemplateAugmentCardBase* Card : ActiveCards)
	{
		Card->SetVisibility(bEnabled ? ESlateVisibility::Visible : ESlateVisibility::HitTestInvisible);
	}
}

void UTemplateAugmentSelectionBase::PlayAppearAll()
{
	SetInteractionEnabled(false);
	AppearedCount = 0;

	for (UTemplateAugmentCardBase* Card : ActiveCards)
	{
		Card->PlayAppear();
	}
}

void UTemplateAugmentSelectionBase::HandleCardAppearFinished(UTemplateAugmentCardBase* Card)
{
	// 마지막 카드까지 다 뜬 뒤에 선택 허용
	if (++AppearedCount >= ActiveCards.Num())
	{
		SetInteractionEnabled(true);
	}
}

void UTemplateAugmentSelectionBase::HandleCardClicked(FName AugmentFName)
{
	// 중복 클릭 방지. 실제 선택 알림은 선택 연출이 끝난 뒤
	SetInteractionEnabled(false);
	ChosenAugmentFName = AugmentFName;

	UTemplateAugmentCardBase* ChosenCard = nullptr;
	for (UTemplateAugmentCardBase* Card : ActiveCards)
	{
		if (Card->GetAugmentFName() == AugmentFName)
		{
			ChosenCard = Card;
		}
		else
		{
			Card->PlayDismiss();
		}
	}

	if (ChosenCard)
	{
		ChosenCard->PlayChosen();
		return;
	}
	OnAugmentChosen.Broadcast(AugmentFName);
}

void UTemplateAugmentSelectionBase::HandleCardChosenFinished(UTemplateAugmentCardBase* Card)
{
	OnAugmentChosen.Broadcast(ChosenAugmentFName);
}
