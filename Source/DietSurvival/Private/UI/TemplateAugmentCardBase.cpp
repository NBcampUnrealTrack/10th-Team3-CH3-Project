#include "UI/TemplateAugmentCardBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

void UTemplateAugmentCardBase::SetupCard(FName InAugmentFName, int32 InSkillLevel)
{
	AugmentFName = InAugmentFName;
}

void UTemplateAugmentCardBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (CardButton)
	{
		CardButton->OnClicked.AddDynamic(this, &UTemplateAugmentCardBase::HandleButtonClicked);
	}
}

void UTemplateAugmentCardBase::PlayAppear()
{
	if (AppearAnim)
	{
		PlayAnimation(AppearAnim);
		return;
	}
	OnAppearFinished.Broadcast(this);
}

void UTemplateAugmentCardBase::PlayChosen()
{
	if (ChosenAnim)
	{
		PlayAnimation(ChosenAnim);
		return;
	}
	OnChosenFinished.Broadcast(this);
}

void UTemplateAugmentCardBase::PlayDismiss()
{
	if (DismissAnim)
	{
		PlayAnimation(DismissAnim);
		return;
	}
	SetVisibility(ESlateVisibility::Hidden);
}

void UTemplateAugmentCardBase::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);

	if (Animation == AppearAnim)
	{
		OnAppearFinished.Broadcast(this);
	}
	else if (Animation == ChosenAnim)
	{
		OnChosenFinished.Broadcast(this);
	}
}

void UTemplateAugmentCardBase::HandleButtonClicked()
{
	OnCardClicked.Broadcast(AugmentFName);
}
