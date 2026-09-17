#pragma once

#include "CoreMinimal.h"
#include "Player/Skill/ActiveSkillBase.h"
#include "Skill_Invincibility.generated.h"

UCLASS()
class DIETSURVIVAL_API USkill_Invincibility : public UActiveSkillBase
{
	GENERATED_BODY()

public:
	USkill_Invincibility();

protected:
	virtual void Activate() override;
	virtual void OnAcquired(const FSkillDeltaRow& DeltaRow) override;
	virtual void OnLevelUp(const FSkillDeltaRow& DeltaRow) override;

	// 무적 지속시간(초)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Invincibility")
	float InvincibilityDuration = 1.f;
};
