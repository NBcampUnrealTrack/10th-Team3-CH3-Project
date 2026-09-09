#include "System/DataTableSubsystem.h"
#include "System/DietGameInstance.h"
#include "System/AugmentsDataRow.h"

UDataTableSubsystem* UDataTableSubsystem::Get(const UObject* WorldContext)
{
	UDietGameInstance* DietGameInstance = UDietGameInstance::Get(WorldContext);
	if (DietGameInstance == nullptr)
	{
		return nullptr;
	}
	return DietGameInstance->GetSubsystem<UDataTableSubsystem>();
}

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

FAugmentsDataRow* UDataTableSubsystem::GetAugmentRowByFName(FName AugmentFName)
{
	FAugmentsDataRow* FoundRow =
		AugmentDataTable->FindRow<FAugmentsDataRow>(AugmentFName, TEXT("SubsystemFindContext"));
	return FoundRow;
}

UDataTable* UDataTableSubsystem::GetEnemyDataTable()
{
	return EnemyDataTable;
}

FEnemyDataRow* UDataTableSubsystem::GetEnemyRowByFName(FName EnemyFName)
{
	FEnemyDataRow* FoundRow =
		EnemyDataTable->FindRow<FEnemyDataRow>(EnemyFName, TEXT("Enemy row date not found"));
	return FoundRow;
}

void UDataTableSubsystem::LoadDataTables(UDataTable* InAugmentDataTable)
{
	AugmentDataTable = InAugmentDataTable;
}

void UDataTableSubsystem::LoadEnemyDataTable(UDataTable* InEnemyDataTable)
{
	EnemyDataTable = InEnemyDataTable;
}
