#pragma once

#include "CoreMinimal.h"
#include "Player/Skill/SkillBase.h"
#include "ActiveSkillBase.generated.h"

// 액티브 스킬의 베이스. 쿨타임 관리, 발동 시도, 발동 성공/실패 등은 이 클래스에서 처리하고, 실제 스킬 효과는 자식 클래스에서 구현
UCLASS()
class DIETSURVIVAL_API UActiveSkillBase : public USkillBase
{
	GENERATED_BODY()

public:
	// 쿨타임을 검사한 뒤 실제로 발동. 발동 성공하면 true, 쿨타임 중이면 false 
	UFUNCTION(BlueprintCallable, Category = "Skill")
	bool TryActivate();

	UFUNCTION(BlueprintPure, Category = "Skill")
	bool IsOnCooldown() const;

	// 남은 쿨타임(초) 반환
	UFUNCTION(BlueprintPure, Category = "Skill")
	float GetRemainingCooldown() const;

protected:
	// 실제 스킬 동작. 자식 클래스가 반드시 구현해야 함 (순수 가상 함수)
	virtual void Activate() PURE_VIRTUAL(UActiveSkillBase::Activate, );

	// 쿨타임(초)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	float Cooldown = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<USoundBase> ActivationSound;

private:
	// 마지막으로 발동한 월드 시간(초). 아주 과거값으로 초기화해서 시작하자마자 사용 가능하게 함
	float LastActivationWorldTime = -100000.f;
};
