#include "Item/SkillAugmentItem.h"
#include "System/DietPlayerState.h"

void ASkillAugmentItem::ActivateItem(APawn* Activator)
{
	UE_LOG(LogTemp, Log, TEXT("[SkillAugmentItem] Activate by %s"), *Activator->GetName());

	ADietPlayerState* DietPlayerState = Activator->GetPlayerState<ADietPlayerState>();
	if (DietPlayerState == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[DigestiveItem] DietPlayerState not found"));
		return;
	}

	DietPlayerState->GainSkillItem();
}

FName ASkillAugmentItem::GetItemType()
{
	return FName("SkillAugment");
}
