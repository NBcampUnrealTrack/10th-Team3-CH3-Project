#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

class USkillBase;

// 플레이어가 보유한 스킬들을 관리하는 컴포넌트
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIETSURVIVAL_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillComponent();

protected:
	virtual void BeginPlay() override;

public:
	// 증강에 나올 수 있는 모든 스킬 후보 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<USkillBase>> AllPossibleSkillClasses;

	// 스킬 인스턴스 배열
	UPROPERTY(BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<USkillBase>> OwnedSkills;

	// 스킬을 획득하거나 이미 가지고 있는 스킬이면 레벨업
	UFUNCTION(BlueprintCallable, Category = "Skill")
	USkillBase* AcquireOrUpgradeSkill(TSubclassOf<USkillBase> SkillClass);

	// 보유 스킬 슬롯(OwnedSkills의 인덱스)으로 발동 시도. 아직 못 배운 스킬은 애초에 배열에 없어서 발동 불가 
	UFUNCTION(BlueprintCallable, Category = "Skill")
	bool TryActivateSkill(int32 SkillIndex);

	// 인덱스로 보유 스킬 인스턴스 자체를 가져옴 (쿨타임/레벨 UI 표시 등에 사용)
	UFUNCTION(BlueprintCallable, Category = "Skill")
	USkillBase* GetOwnedSkill(int32 SkillIndex) const;
};
