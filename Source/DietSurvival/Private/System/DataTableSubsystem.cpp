#include "System/DataTableSubsystem.h"
#include "System/DietGameInstance.h"
#include "System/AugmentsDataRow.h"
#include "System/EnemyDataRow.h"

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

	if (FoundRow == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DataTableSubsystem] Data row not found"));
		return nullptr;
	}
	return FoundRow;
}

FText UDataTableSubsystem::GetAugmentUIName(FName AugmentFName)
{
	FAugmentsDataRow* FoundRow = AugmentDataTable->FindRow<FAugmentsDataRow>(AugmentFName, TEXT("Subsystem: GetAugmentUIName"));
	return FoundRow->AugmentUIName;
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

void UDataTableSubsystem::LoadAugmentDataTable(UDataTable* InAugmentDataTable)
{
	AugmentDataTable = InAugmentDataTable;
}

void UDataTableSubsystem::LoadEnemyDataTable(UDataTable* InEnemyDataTable)
{
	EnemyDataTable = InEnemyDataTable;
}
