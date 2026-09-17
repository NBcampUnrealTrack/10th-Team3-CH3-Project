#pragma once

#include "CoreMinimal.h"
#include "Player/Skill/PassiveSkillBase.h"
#include "Skill_AuraSkill.generated.h"

class AAuraOrbActor;

UCLASS()
class DIETSURVIVAL_API USkill_AuraSkill : public UPassiveSkillBase
{
	GENERATED_BODY()

public:
	USkill_AuraSkill();

protected:
	virtual void OnAcquired(const FSkillDeltaRow& DeltaRow) override;
	virtual void OnLevelUp(const FSkillDeltaRow& DeltaRow) override;

	// 실제로 스폰할 오브 액터 클래스. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Aura")
	TSubclassOf<AAuraOrbActor> OrbActorClass;

	// 오브가 도는 반경 (cm) 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Aura")
	float OrbitRadius = 200.f;

	// 오브 회전 속도 (초당 각도) 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Aura")
	float OrbitSpeed = 90.f;

	// 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Aura")
	float Damage = 0.f;

private:
	UPROPERTY()
	TArray<TObjectPtr<AAuraOrbActor>> SpawnedOrbs;

	// 오브를 하나 새로 스폰해서 SpawnedOrbs에 등록 
	void SpawnOrb();

	// 오브 개수가 바뀔 때, 모든 오브가 원 위에 균등한 간격으로 재배치되도록 각도를 다시 나눔 
	void RedistributeOrbAngles();

	// 보유 중인 모든 오브에게 현재 데미지 배율을 다시 전달 
	void UpdateAllOrbDamage();

};
