#pragma once

#include "UI/TemplateAugmentCardBase.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AugmentCardWidget.generated.h"

UCLASS()
class DIETSURVIVAL_API UAugmentCardWidget : public UTemplateAugmentCardBase
{
	GENERATED_BODY()

public:
	void SetupCard(FName InAugmentFName, int32 InAugmentLevel) override;
};
