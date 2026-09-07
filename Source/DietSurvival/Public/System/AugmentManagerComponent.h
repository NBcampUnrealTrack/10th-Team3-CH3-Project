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
	TWeakObjectPtr<UDataTable> AugmentsData;

	TMap<FName, TArray<int32>> AugmentsMap;
};
