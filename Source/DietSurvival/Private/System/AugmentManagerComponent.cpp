#include "System/AugmentManagerComponent.h"
#include "System/DietGameState.h"
#include "System/AugmentsDataRow.h"
#include "System/DataTableSubsystem.h"
#include "Algo/RandomShuffle.h"

UAugmentManagerComponent::UAugmentManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UAugmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UDataTableSubsystem* DataTableSubsystem = UDataTableSubsystem::Get(this))
	{
		AugmentsData = DataTableSubsystem->GetAugmentDataTable();
	}

	if (AugmentsData.IsValid())
	{
		TArray<FAugmentsDataRow*> AllAugments;
		AugmentsData->GetAllRows(TEXT("InitAugmentsContext"), AllAugments);

		for (const auto Augment : AllAugments)
		{
			AugmentsMap.Add(Augment->AugmentFName, TArray<int32>({ 0, Augment->MaxAugmentLevel }));
		}

		// Test
		//for (const auto& [Name, Levels] : AugmentsMap)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("%s - 현재 레벨: %d, 최대 레벨: %d"), *Name.ToString(), Levels[0], Levels[1]);
		//}
	}

	// Test
	//StartAugment();
}

void UAugmentManagerComponent::StartAugment()
{
	TArray<FName> SelectedAugments = SelectRandomAugments();

	// test log
	for (int32 i = 0; i < SelectedAugments.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d번째 증강: %s"), i + 1, *SelectedAugments[i].ToString());
	}
}

TArray<FName> UAugmentManagerComponent::SelectRandomAugments()
{
	TArray<FName> Candidates;
	for (const auto& [Name, Levels] : AugmentsMap)
	{
		int32 CurrentLevel = Levels[0];
		int32 MaxLevel = Levels[1];
		if (CurrentLevel < MaxLevel)
		{
			Candidates.Add(Name);
		}
	}

	if (Candidates.Num() <= 3)
	{
		return Candidates;
	}

	Algo::RandomShuffle(Candidates);
	TArray<FName> Selection;
	Selection.Append(&Candidates[0], 3);
	return Selection;
}
