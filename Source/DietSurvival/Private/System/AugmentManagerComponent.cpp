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
			AugmentsMap.Add(Augment->AugmentFName, FAugmentData(0, Augment->MaxAugmentLevel, Augment->PickWeight));
		}

		// Test
		if (AugmentsMap.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Map 초기화 안 됨"));
		}
		for (const auto& [Name, Data] : AugmentsMap)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s - 현재 레벨: %d, 최대 레벨: %d, 가중치: %.0f"), *Name.ToString(), Data.CurrentLevel, Data.MaxLevel, Data.Weight);
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
		FAugmentData& Data = AugmentsMap[ChosenAugmentFName];
		Data.CurrentLevel++;   // 증강 레벨 +1
		if (Data.CurrentLevel != Data.MaxLevel)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s 증강 레벨업: Lv.%d"), *ChosenAugmentFName.ToString(), Data.CurrentLevel);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s 증강 최대 레벨 도달: Lv.%d"), *ChosenAugmentFName.ToString(), Data.CurrentLevel);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("큰일남. 증강 목록에 없는 걸 선택해버림. 이 로그 뜨면 망함."));
	}
}

TArray<TTuple<FName, int32>> UAugmentManagerComponent::SelectRandomAugments()
{
	TArray<TTuple<FName, FAugmentData>> Candidates;
	TArray<TTuple<FName, int32>> ResultArray;

	// 최대 레벨에 도달하지 않은 증강만 Candidates에 추가.
	for (const auto& [Name, Data] : AugmentsMap)
	{
		if (Data.CurrentLevel < Data.MaxLevel)
		{
			Candidates.Add(MakeTuple(Name, Data));
		}
	}
	Algo::RandomShuffle(Candidates);

	// 선택 가능한 증강이 3개 이하면 그대로 반환
	if (Candidates.Num() <= 3)
	{
		for (const auto& [Name, Data] : Candidates)
		{
			ResultArray.Add(MakeTuple(Name, Data.CurrentLevel));
		}
		return ResultArray;
	}

	// 선택 가능한 증강이 4개 이상이면 가중치에 기반해 선택
	for (int32 i = 0; i < 3 && 0 < Candidates.Num(); i++)
	{
		float WeightSum = 0.f;
		for (const auto& [Name, Data] : Candidates)
		{
			WeightSum += Data.Weight;
		}
		if (FMath::IsNearlyZero(WeightSum)) { break; }

		float RandomNumber = FMath::RandRange(0.f, WeightSum);
		float ChanceAccumulate = 0.f;
		for (int32 Index = 0; Index < Candidates.Num(); Index++)
		{
			ChanceAccumulate += Candidates[Index].Get<1>().Weight;
			if (RandomNumber <= ChanceAccumulate)
			{
				ResultArray.Add(MakeTuple(Candidates[Index].Get<0>(), Candidates[Index].Get<1>().CurrentLevel));
				Candidates.RemoveAt(Index);
				break;
			}
		}
	}
	return ResultArray;
}

FAugmentData::FAugmentData()
	: CurrentLevel(0), MaxLevel(0), Weight(0)
{
}

FAugmentData::FAugmentData(int32 InCurrentLevel, int32 InMaxLevel, float InWeight)
	: CurrentLevel(InCurrentLevel), MaxLevel(InMaxLevel), Weight(InWeight)
{
}
