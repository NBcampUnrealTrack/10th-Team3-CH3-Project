#include "UI/TemplateAugmentCardBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

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

void UTemplateAugmentCardBase::HandleButtonClicked()
{
	OnCardClicked.Broadcast(AugmentFName);
}
