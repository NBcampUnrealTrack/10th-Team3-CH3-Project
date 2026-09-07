#pragma once

#include "CoreMinimal.h"
#include "AugmentsDataRow.generated.h"

USTRUCT(BlueprintType)
struct FAugmentsDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 해당 증강의 최대 레벨. DeltaPerAugmentLevel의 길이와 같아야 함.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAugmentLevel;

	// 해당 증강의 레벨별 능력치 증가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> DeltaPerAugmentLevel;

	// 해당 증강의 설명. 예) "공격력이 {0}증가합니다."
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
};
