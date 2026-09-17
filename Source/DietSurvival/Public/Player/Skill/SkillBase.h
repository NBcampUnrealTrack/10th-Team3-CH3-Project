#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillBase.generated.h"

class APlayerCharacter;

// 모든 스킬의 베이스 클래스. 쿨타임 같은 공통적인 로직 처리
UCLASS(Abstract, Blueprintable, EditInlineNew)
class DIETSURVIVAL_API USkillBase : public UObject
{
	GENERATED_BODY()

public:
	// 스킬 컴포넌트가 인스턴스 생성 후 딱 한 번 호출해서 소유자(플레이어) 등록
	void InitializeSkill(APlayerCharacter* InOwner);

	// 스킬 이름 반환
	UFUNCTION(BlueprintPure, Category = "Skill")
	FName GetSkillName() const { return SkillName; }

	// 스킬 레벨업
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void LevelUpSkill();

	// 스킬 레벨 반환
	UFUNCTION(BlueprintPure, Category = "Skill")
	int32 GetLevel() const { return Level; }


protected:
	// 스킬 레벨업 시 호출되는 가상 함수. 자식 클래스에서 오버라이드
	virtual void OnLevelUp() { }

	// 스킬 이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	FName SkillName = TEXT("Skill");

	// 스킬 레벨
	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	int32 Level = 1;

	// 이 스킬을 소유한 플레이어. InitializeSkill에서 한 번 설정되고 계속 재사용됨. 단순 참조이기에 WeakPointer
	UPROPERTY()
	TWeakObjectPtr<APlayerCharacter> OwnerCharacter;
};
