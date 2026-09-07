#include "System/AugmentManagerComponent.h"
#include "System/DietGameState.h"
#include "System/AugmentsDataRow.h"

UAugmentManagerComponent::UAugmentManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UAugmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ADietGameState* DietGameState = Cast<ADietGameState>(GetWorld()->GetGameState()))
	{
		// DataTableSubsystem이 갖고있는 증강 데이터 테이블을 AugmentsData에 연결
		// 예)
		//AugmentsData = DataTableSubsystem->AugmentsDataTable;
	}

	if (AugmentsData.IsValid())
	{
		TArray<FAugmentsDataRow*> AllAugments;
		AugmentsData->GetAllRows(TEXT("InitAugmentsContext"), AllAugments);

		for (const auto Augment : AllAugments)
		{
			AugmentsMap.Add(Augment->AugmentFName, TArray<int32>({ 0, Augment->MaxAugmentLevel }));
		}
	}
}

void UAugmentManagerComponent::StartAugment()
{
}

TArray<FName> UAugmentManagerComponent::SelectRandomAugments()
{
	return TArray<FName>();
}
