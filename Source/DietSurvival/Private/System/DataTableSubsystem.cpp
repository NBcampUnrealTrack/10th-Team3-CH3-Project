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

float UDataTableSubsystem::GetAugmentDelta(FName AugmentFName, int32 AugmentLevel)
{
	FAugmentsDataRow* FoundRow = AugmentDataTable->FindRow<FAugmentsDataRow>(AugmentFName, TEXT("Subsystem: GetAugmentDelta"));
	return FoundRow->DeltaPerAugmentLevel[AugmentLevel];
}

FText UDataTableSubsystem::GetAugmentDescription(FName AugmentFName)
{
	FAugmentsDataRow* FoundRow = AugmentDataTable->FindRow<FAugmentsDataRow>(AugmentFName, TEXT("Subsystem: GetAugmentDescription"));
	return FoundRow->Description;
}

EPlayerStatType UDataTableSubsystem::GetAugmentStatType(FName AugmentFName)
{
	FAugmentsDataRow* FoundRow = AugmentDataTable->FindRow<FAugmentsDataRow>(AugmentFName, TEXT("Subsystem: GetAugmentStatType"));
	return FoundRow->StatType;
}

bool UDataTableSubsystem::IsAugmentShowFractionalDigit(FName AugmentFName)
{
	FAugmentsDataRow* FoundRow = AugmentDataTable->FindRow<FAugmentsDataRow>(AugmentFName, TEXT("Subsystem: IsAugmentShowFractionalDigit"));
	return FoundRow->bShowFractionalDigit;
}

void UDataTableSubsystem::LoadDataTables(UDataTable* InAugmentDataTable)
{
	AugmentDataTable = InAugmentDataTable;
}
