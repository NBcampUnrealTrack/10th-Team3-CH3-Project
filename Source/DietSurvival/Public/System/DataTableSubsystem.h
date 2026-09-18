#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataTableSubsystem.generated.h"

struct FAugmentsDataRow;
struct FEnemyDataRow;
struct FSkillDeltaRow;
enum class EPlayerStatType : uint8;
class USkillBase;

UCLASS()
class DIETSURVIVAL_API UDataTableSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UDataTableSubsystem* Get(const UObject* WorldContext);

public:
	// lifecycle

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	// getters

	// ----- 몬스터 스폰 관련 -----

	UDataTable* GetEnemyDataTable();

	FEnemyDataRow* GetEnemyRowByFName(FName EnemyFName);

	// ----- 스탯 증강 관련 -----

	// TWeakObjectPtr<UDataTable>로 받아서 사용
	UDataTable* GetAugmentDataTable();

	FAugmentsDataRow* GetAugmentRowByFName(FName AugmentFName);

	FText GetAugmentUIName(FName AugmentFName);

	// 증강의 해당 레벨에서의 능력치 상승량 반환
	float GetAugmentDelta(FName AugmentFName, int32 AugmentLevel);

	FText GetAugmentDescription(FName AugmentFName);

	EPlayerStatType GetAugmentStatType(FName AugmentFName);

	bool IsAugmentShowFractionalDigit(FName AugmentFName);

	// ----- 스킬 증강 관련 -----
	UDataTable* GetSkillDataTable();

	FText& GetSkillUIName(FName SkillFName) const;

	FSkillDeltaRow& GetSkillDeltaRow(FName SkillFName, int32 Level) const;

	FText GetSkillDescription(FName SkillFName, int32 Index) const;

	TSubclassOf<USkillBase> GetSkillClass(FName SkillFName) const;

public:
	// functions

	void LoadAugmentDataTable(UDataTable* InAugmentDataTable);
	void LoadSkillDataTable(UDataTable* InSkillDataTable);
	void LoadEnemyDataTable(UDataTable* InEnemyDataTable);

private:
	// variables

	UPROPERTY()
	TObjectPtr<UDataTable> AugmentDataTable;

	UPROPERTY()
	TObjectPtr<UDataTable> SkillDataTable;

	UPROPERTY()
	TObjectPtr<UDataTable> EnemyDataTable;
};
