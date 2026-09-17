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
	virtual void OnAcquired() override;
	virtual void OnLevelUp() override;

	// 실제로 스폰할 오브 액터 클래스. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Aura")
	TSubclassOf<AAuraOrbActor> OrbActorClass;

	// 오브가 도는 반경 (cm) 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Aura")
	float OrbitRadius = 200.f;

	// 오브 회전 속도 (초당 각도) 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Aura")
	float OrbitSpeed = 90.f;

	// StatComponent의 AttackPower에 곱해질 배율 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Aura")
	float DamageMultiplier = 1.f;

	// 레벨업마다(매 레벨) 배율이 이만큼 증가 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Aura")
	float DamageMultiplierIncreasePerLevel = 0.3f;

	// 오브가 몇 레벨마다 하나씩 늘어나는지 (기본: 3레벨마다) 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Aura")
	int32 LevelsPerExtraOrb = 3;

private:
	UPROPERTY()
	TArray<TObjectPtr<AAuraOrbActor>> SpawnedOrbs;

	// 현재 레벨 기준으로 있어야 할 오브 개수 계산 
	int32 CalculateDesiredOrbCount() const;

	// 오브를 하나 새로 스폰해서 SpawnedOrbs에 등록 
	void SpawnOrb();

	// 오브 개수가 바뀔 때, 모든 오브가 원 위에 균등한 간격으로 재배치되도록 각도를 다시 나눔 
	void RedistributeOrbAngles();

	// 보유 중인 모든 오브에게 현재 데미지 배율을 다시 전달 
	void UpdateAllOrbDamage();

};
