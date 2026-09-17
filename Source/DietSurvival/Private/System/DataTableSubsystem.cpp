#include "System/DataTableSubsystem.h"
#include "System/DietGameInstance.h"
#include "System/AugmentsDataRow.h"
#include "System/SkillDataRow.h"
#include "System/EnemyDataRow.h"
#include "Player/Skill/SkillBase.h"

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

UDataTable* UDataTableSubsystem::GetSkillDataTable()
{
	return SkillDataTable;
}

FText& UDataTableSubsystem::GetSkillUIName(FName SkillFName) const
{
	FSkillDataRow* FoundRow = SkillDataTable->FindRow<FSkillDataRow>(SkillFName, TEXT("Subsystem: GetSkillUIName"));
	return FoundRow->SkillUIName;
}

FSkillDeltaRow& UDataTableSubsystem::GetSkillDeltaRow(FName SkillFName, int32 Level) const
{
	FSkillDataRow* FoundRow = SkillDataTable->FindRow<FSkillDataRow>(SkillFName, TEXT("Subsystem: GetSkillDeltaRow"));
	return FoundRow->DeltaPerSkillLevel[Level];
}

FText UDataTableSubsystem::GetSkillDescription(FName SkillFName, int32 Level) const
{
	FSkillDataRow* FoundRow = SkillDataTable->FindRow<FSkillDataRow>(SkillFName, TEXT("Subsystem: GetSkillDescriptionIndex"));
	int32 Index = FoundRow->DescriptionIndex[Level];
	if (0 <= Index && Index < FoundRow->Descriptions.Num())
	{
		return FoundRow->Descriptions[Index];
	}
	else { return FText::FromString(FString::Printf(TEXT("Invalid Index"))); }
}

TSubclassOf<USkillBase> UDataTableSubsystem::GetSkillClass(FName SkillFName) const
{
	FSkillDataRow* FoundRow = SkillDataTable->FindRow<FSkillDataRow>(SkillFName, TEXT("Subsystem: GetSkillClass"));
	return FoundRow->SkillClass;
}

void UDataTableSubsystem::LoadAugmentDataTable(UDataTable* InAugmentDataTable)
{
	AugmentDataTable = InAugmentDataTable;
}

void UDataTableSubsystem::LoadSkillDataTable(UDataTable* InSkillDataTable)
{
	SkillDataTable = InSkillDataTable;
}

void UDataTableSubsystem::LoadEnemyDataTable(UDataTable* InEnemyDataTable)
{
	EnemyDataTable = InEnemyDataTable;
}
