#include "System/AugmentsDataRow.h"

void FAugmentsDataRow::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
	Super::OnDataTableChanged(InDataTable, InRowName);
	AugmentFName = InRowName;
}
