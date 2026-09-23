#pragma once

#include "CoreMinimal.h"
#include "Player/Skill/ActiveSkillBase.h"
#include "Skill_AreaAttack.generated.h"

// 광역기: 발동 시 주변 반경 내 모든 적(Enemy 태그)에게 즉시 데미지
UCLASS()
class DIETSURVIVAL_API USkill_AreaAttack : public UActiveSkillBase
{
	GENERATED_BODY()

public:
	USkill_AreaAttack();

protected:
	virtual void Activate() override;
	virtual void OnAcquired(const FSkillDeltaRow& DeltaRow) override;
	virtual void OnLevelUp(const FSkillDeltaRow& DeltaRow) override;

	// 데미지 판정 반경 (cm)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|AreaAttack")
	float Radius = 400.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	float Damage = 0.f;

	// 레벨업마다 반경이 이만큼 증가 (cm)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|AreaAttack")
	float RadiusIncreasePerLevel = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|AreaAttack")
	float CooldownDecreasePerLevel = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|AreaAttack")
	TObjectPtr<UParticleSystem> ExplosionEffect;
};
