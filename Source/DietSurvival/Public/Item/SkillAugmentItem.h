#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "SkillAugmentItem.generated.h"

UCLASS()
class DIETSURVIVAL_API ASkillAugmentItem : public ABaseItem
{
	GENERATED_BODY()


public:
	virtual void ActivateItem(APawn* Activator) override;
	virtual FName GetItemType() override;
};
