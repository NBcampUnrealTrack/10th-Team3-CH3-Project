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
	virtual void OnLevelUp() override;

	// 데미지 판정 반경 (cm)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|AreaAttack")
	float Radius = 400.f;

	// StatComponent의 AttackPower에 곱해질 배율
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|AreaAttack")
	float DamageMultiplier = 1.f;

	// 레벨업마다 배율이 이만큼 증가
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|AreaAttack")
	float DamageMultiplierIncreasePerLevel = 0.5f;

	// 레벨업마다 반경이 이만큼 증가 (cm)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|AreaAttack")
	float RadiusIncreasePerLevel = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|AreaAttack")
	float CooldownDecreasePerLevel = 1.f;
};
