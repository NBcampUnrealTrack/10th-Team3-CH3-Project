#pragma once

#include "CoreMinimal.h"
#include "Player/Skill/SkillBase.h"
#include "PassiveSkillBase.generated.h"

UCLASS()
class DIETSURVIVAL_API UPassiveSkillBase : public USkillBase
{
	GENERATED_BODY()

public:
	// 스킬 획득 후 한번만 호출되는 함수
	virtual void OnAcquired(const FSkillDeltaRow& DeltaRow) PURE_VIRTUAL(UPassiveSkillBase::OnAcquired, );
};
