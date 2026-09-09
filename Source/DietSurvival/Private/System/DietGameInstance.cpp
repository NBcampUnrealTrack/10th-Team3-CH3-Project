#include "System/DietGameInstance.h"
#include "System/DataTableSubsystem.h"
#include "Kismet/GameplayStatics.h"

UDietGameInstance* UDietGameInstance::Get(const UObject* WorldContext)
{
	return Cast<UDietGameInstance>(UGameplayStatics::GetGameInstance(WorldContext));
}

UDietGameInstance::UDietGameInstance()
{
}

void UDietGameInstance::Init()
{
	Super::Init();

	if (UDataTableSubsystem* DataTableSubsystem = GetSubsystem<UDataTableSubsystem>())
	{
		DataTableSubsystem->LoadDataTables(AugmentDataTable);
	}
}
