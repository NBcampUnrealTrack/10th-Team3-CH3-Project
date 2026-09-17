#include "System/SkillDataRow.h"

void FSkillDataRow::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
	Super::OnDataTableChanged(InDataTable, InRowName);
	SkillFName = InRowName;
	MaxSkillLevel = DeltaPerSkillLevel.Num();
}
