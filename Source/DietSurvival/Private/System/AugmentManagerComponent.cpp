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

	UE_LOG(LogTemp, Warning, TEXT("AugmentManagerComponent BeginPlay()"));

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
		if (AugmentsMap.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Map 초기화 안 됨"));
		}
		for (const auto& [Name, Levels] : AugmentsMap)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s - 현재 레벨: %d, 최대 레벨: %d"), *Name.ToString(), Levels[0], Levels[1]);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("증강 데이터 테이블을 가져올 수 없음"));
	}

	// Test
	//StartAugment();
}

void UAugmentManagerComponent::AugmentLevelUp(FName ChosenAugmentFName)
{
	if (AugmentsMap.Contains(ChosenAugmentFName))
	{
		TArray<int32>& Levels = AugmentsMap[ChosenAugmentFName];
		Levels[0]++;   // 증강 레벨 +1
		if (Levels[0] != Levels[1])
		{
			UE_LOG(LogTemp, Warning, TEXT("%s 증강 레벨업: Lv.%d"), *ChosenAugmentFName.ToString(), Levels[0]);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s 증강 최대 레벨 도달: Lv.%d"), *ChosenAugmentFName.ToString(), Levels[0]);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("큰일남. 증강 목록에 없는 걸 선택해버림. 이 로그 뜨면 망함."));
	}
}

TArray<TTuple<FName, int32>> UAugmentManagerComponent::SelectRandomAugments()
{
	TArray<TTuple<FName, int32>> Candidates;
	for (const auto& [Name, Levels] : AugmentsMap)
	{
		int32 CurrentLevel = Levels[0];
		int32 MaxLevel = Levels[1];
		if (CurrentLevel < MaxLevel)
		{
			Candidates.Add(MakeTuple(Name, CurrentLevel));
		}
	}
	Algo::RandomShuffle(Candidates);

	if (Candidates.Num() <= 3)
	{
		return Candidates;
	}
	TArray<TTuple<FName, int32>> Selection;
	Selection.Append(&Candidates[0], 3);
	return Selection;
}
