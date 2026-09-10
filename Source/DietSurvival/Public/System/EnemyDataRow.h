#pragma once

#include "CoreMinimal.h"
#include "EnemyDataRow.generated.h" // 리플렉션용 제너레이트 파일

class ADietEnemyBase;

USTRUCT(BlueprintType)
struct FEnemyDataRow : public FTableRowBase // 데이터테이블 연동 시 FTableRowBase 상속 가능
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<ADietEnemyBase> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 PowerAttack;
};
