#include "Item/SkillAugmentItem.h"

void ASkillAugmentItem::ActivateItem(APawn* Activator)
{
	UE_LOG(LogTemp, Log, TEXT("[SkillAugmentItem] Activate by %s"), *Activator->GetName());
}

FName ASkillAugmentItem::GetItemType()
{
	return FName("SkillAugment");
}
