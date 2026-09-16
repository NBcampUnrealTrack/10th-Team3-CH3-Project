#pragma once

#include "CoreMinimal.h"
#include "SkillDataRow.generated.h"

// DeltaPerSkillLevel의 원소로 사용할 배열을 감싸는 구조체.
USTRUCT(BlueprintType)
struct FSkillDeltaRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Row;
};

USTRUCT(BlueprintType)
struct FSkillDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 해당 스킬의 FName. RowName을 변경하면 자동으로 갱신됨.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName SkillFName;

	// UI에 표시할 해당 스킬의 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillUIName;

	// 해당 스킬의 최대 레벨. DeltaPerSkillLevel의 길이와 맞도록 자동 갱신.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MaxSkillLevel;

	// 해당 스킬의 레벨별 능력치 증가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSkillDeltaRow> DeltaPerSkillLevel;

	// 각 레벨에서 어떤 Description을 카드에 보여줄지 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> DescriptionIndex;

	// 스킬의 설명. Index0에는 최초 획득시 설명을 기재. Index1부터 스킬 레벨업 시 보여줄 설명 기재.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> Descriptions;

	// 자동으로 SkillFName, MaxSkillLevel의 값을 갱신.
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
};
