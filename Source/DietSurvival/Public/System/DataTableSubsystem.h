#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataTableSubsystem.generated.h"

struct FAugmentsDataRow;
enum class EPlayerStatType : uint8;

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

	// TWeakObjectPtr<UDataTable>로 받아서 사용
	UDataTable* GetAugmentDataTable();

	FAugmentsDataRow* GetAugmentRowByFName(FName AugmentFName);

	// 증강의 해당 레벨에서의 능력치 상승량 반환
	float GetAugmentDelta(FName AugmentFName, int32 AugmentLevel);

	FText GetAugmentDescription(FName AugmentFName);

	EPlayerStatType GetAugmentStatType(FName AugmentFName);

	bool IsAugmentShowFractionalDigit(FName AugmentFName);

public:
	// functions

	void LoadDataTables(UDataTable* InAugmentDataTable);

private:
	// variables

	UPROPERTY()
	TObjectPtr<UDataTable> AugmentDataTable;
};
