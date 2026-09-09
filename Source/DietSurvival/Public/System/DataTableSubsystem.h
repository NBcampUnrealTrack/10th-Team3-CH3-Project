#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataTableSubsystem.generated.h"

struct FAugmentsDataRow;

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

public:
	// functions

	void LoadDataTables(UDataTable* InAugmentDataTable);

private:
	// variables

	UPROPERTY()
	TObjectPtr<UDataTable> AugmentDataTable;
};
