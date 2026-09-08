#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AugmentManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIETSURVIVAL_API UAugmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAugmentManagerComponent();

	// 증강 시작
	void StartAugment();

	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	// AugmentsMap에서 최대 증강 레벨에 도달하지 않은 랜덤한 증강을 최대 3개 뽑아서 반환
	TArray<FName> SelectRandomAugments();

protected:
	// DataTableSubsystem이 갖고있는 증강 데이터 테이블을 참조
	TWeakObjectPtr<UDataTable> AugmentsData;

	// 플레이어의 증강 정보를 저장. 예) [ AttackAugment, {CurrentLevel: 2, MaxLevel: 10} ]
	TMap<FName, TArray<int32>> AugmentsMap;
};
