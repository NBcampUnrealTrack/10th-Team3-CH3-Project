#pragma once

#include "UI/TemplateAugmentCardBase.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillCardWidget.generated.h"

UCLASS()
class DIETSURVIVAL_API USkillCardWidget : public UTemplateAugmentCardBase
{
	GENERATED_BODY()

public:
	void SetupCard(FName InSkillFName, int32 InSkillLevel);
};
