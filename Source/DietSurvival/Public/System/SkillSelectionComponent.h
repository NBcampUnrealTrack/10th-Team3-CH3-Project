#pragma once

#include "System/TemplateAugSelectionCompBase.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillSelectionComponent.generated.h"

UCLASS( ClassGroup=(DietSurvival), meta=(BlueprintSpawnableComponent) )
class DIETSURVIVAL_API USkillSelectionComponent : public UTemplateAugSelectionCompBase
{
	GENERATED_BODY()

public:
	USkillSelectionComponent();
	
protected:
	UFUNCTION()
	void HandleSkillUp();

	virtual void TryBindToDelegate() override;
	virtual void LoadCandidates() override;
	virtual void InitializeSelectionWidget() override;
	virtual void ApplyAugment(FName AugmentFName, int32 Level) override;
};
