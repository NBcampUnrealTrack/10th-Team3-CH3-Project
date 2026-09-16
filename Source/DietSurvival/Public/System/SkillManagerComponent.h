#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillManagerComponent.generated.h"

USTRUCT(BlueprintType)
struct FSkillData
{
	GENERATED_BODY()

public:
	int32 CurrentLevel = 0;
	int32 MaxLevel = 0;

	FSkillData() = default;
	FSkillData(int32 InCurrentLevel, int32 InMaxLevel);
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIETSURVIVAL_API USkillManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillManagerComponent();

	// 최대 레벨에 도달하지 않은 스킬을 반환
	TArray<TTuple<FName, int32>> GetSkillList();

	// 스킬 레벨을 1 증가시킴
	void SkillLevelUp(FName ChosenSkillFName);

protected:
	virtual void BeginPlay() override;

private:
	// DataTableSubsystem이 갖고있는 스킬 데이터 테이블을 참조
	TWeakObjectPtr<UDataTable> SkillData;

	// 플레이어의 스킬 정보를 저장. 예) [ HoveringSkill, {CurrentLevel: 2, MaxLevel: 10} ]
	TMap<FName, FSkillData> SkillMap;
};
