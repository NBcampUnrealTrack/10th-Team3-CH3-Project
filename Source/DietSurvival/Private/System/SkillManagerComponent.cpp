#include "System/SkillManagerComponent.h"
#include "System/DataTableSubsystem.h"
#include "System/SkillDataRow.h"

USkillManagerComponent::USkillManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

TArray<TTuple<FName, int32>> USkillManagerComponent::GetSkillList()
{
	TArray<TTuple<FName, int32>> ResultArray;
	for (const auto& [Name, Data] : SkillMap)
	{
		if (Data.CurrentLevel < Data.MaxLevel)
		{
			ResultArray.Add(MakeTuple(Name, Data.CurrentLevel));
		}
	}
	return ResultArray;
}

void USkillManagerComponent::SkillLevelUp(FName ChosenSkillFName)
{
	FSkillData& Data = SkillMap[ChosenSkillFName];
	Data.CurrentLevel++;
	if (Data.CurrentLevel != Data.MaxLevel)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s 스킬 레벨업: Lv.%d"), *ChosenSkillFName.ToString(), Data.CurrentLevel);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s 스킬 최대 레벨 도달: Lv.%d"), *ChosenSkillFName.ToString(), Data.CurrentLevel);
	}
}

void USkillManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UDataTableSubsystem* DTSubsystem = UDataTableSubsystem::Get(this))
	{
		SkillData = DTSubsystem->GetSkillDataTable();
	}

	// SkillMap을 초기화
	if (SkillData.IsValid())
	{
		TArray<FSkillDataRow*> AllSkills;
		SkillData->GetAllRows(TEXT("InitSkillsContext"), AllSkills);

		for (const auto Skill : AllSkills)
		{
			SkillMap.Add(Skill->SkillFName, FSkillData(0, Skill->MaxSkillLevel));
		}

		// 디버그 로그
		if (SkillMap.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Map 초기화 안 됨"));
		}
		for (const auto& [Name, Data] : SkillMap)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s - 현재 레벨: %d, 최대 레벨: %d"), *Name.ToString(), Data.CurrentLevel, Data.MaxLevel);
		}
	}
}

FSkillData::FSkillData(int32 InCurrentLevel, int32 InMaxLevel)
	: CurrentLevel(InCurrentLevel), MaxLevel(InMaxLevel)
{
}
