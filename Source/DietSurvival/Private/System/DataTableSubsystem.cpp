#include "System/DataTableSubsystem.h"

void UDataTableSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UDataTableSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

UDataTable* UDataTableSubsystem::GetAugmentDataTable()
{
	return AugmentDataTable;
}

void UDataTableSubsystem::LoadDataTables(UDataTable* InAugmentDataTable)
{
	AugmentDataTable = InAugmentDataTable;
}
