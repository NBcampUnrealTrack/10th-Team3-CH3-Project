#pragma once

#include "System/TemplateAugSelectionCompBase.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AugmentSelectionComponent.generated.h"

UCLASS( ClassGroup=(DietSurvival), meta=(BlueprintSpawnableComponent) )
class DIETSURVIVAL_API UAugmentSelectionComponent : public UTemplateAugSelectionCompBase
{
	GENERATED_BODY()

public:
	UAugmentSelectionComponent();

protected:
	UFUNCTION()
	void HandleLevelUp(int32 NewLevel);

	UFUNCTION()
	void HandleRerollPressed();

	virtual void TryBindToDelegate() override;
	virtual void LoadCandidates() override;
	virtual void InitializeSelectionWidget() override;
	virtual void ApplyAugment(FName AugmentFName, int32 Level) override;

};
