#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TemplateAugSelectionCompBase.generated.h"

class ADietPlayerState;

UCLASS( ClassGroup=(DietSurvival), meta=(BlueprintSpawnableComponent) )
class DIETSURVIVAL_API UTemplateAugSelectionCompBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UTemplateAugSelectionCompBase();

	void StartSelection();

protected:
	// 플레이어가 증강을 선택했을 때 실행될 핸들러. ActiveWidgetInstance의 증강 선택 이벤트에 바인딩한다.
	UFUNCTION()
	virtual void HandleAugmentChosen(FName ChosenAugmentFName);

	// PlayerState를 캐시하고 OnLevelUp, OnSkillUp 등의 이벤트에 핸들러 바인딩
	UFUNCTION()
	virtual void TryBindToDelegate();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	// CachedCandidates에 최대 3개의 증강 저장
	virtual void LoadCandidates();
	// InitializeCards(), Delegate 바인딩 수행
	virtual void InitializeSelectionWidget();
	// 플레이어가 선택한 증강을 실제로 적용
	virtual void ApplyAugment(FName AugmentFName, int32 Level);

	void FinishSelection();

	FORCEINLINE APlayerController* GetOwningController() const { return Cast<APlayerController>(GetOwner()); }

	UPROPERTY()
	TObjectPtr<ADietPlayerState> CachedPS;

	UPROPERTY(EditDefaultsOnly, Category = "Augment")
	TSubclassOf<UUserWidget> SelectionWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> ActiveWidgetInstance;

	TArray<TTuple<FName, int32>> CachedCandidates;
	int32 PendingAugmentCount = 0;
	bool bIsSelecting = false;
};
