#pragma once

#include "CoreMinimal.h"
#include "AugmentsDataRow.generated.h"

USTRUCT(BlueprintType)
struct FAugmentsDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 해당 증강의 이름 FName. UI에 표시할 내용은 아님.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName AugmentFName;

	// 해당 증강의 최대 레벨. DeltaPerAugmentLevel의 길이와 같아야 함.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAugmentLevel;

	// 해당 증강의 레벨별 능력치 증가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> DeltaPerAugmentLevel;

	// 해당 증강의 설명. 예) "공격력이 {0}증가합니다."
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	// 자동으로 AugmentFName의 값을 RowName으로 채움.
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
};
