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

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	UFUNCTION()
	virtual void HandleAugmentChosen(FName ChosenAugmentFName) = 0;

	UFUNCTION()
	virtual void TryBindToDelegate() = 0;

private:
	void StartSelection();
	void FinishSelection();

	// CachedCandidates에 최대 3개의 증강 저장
	virtual void LoadCandidates() = 0;
	// InitializeCards(), Delegate 바인딩 수행
	virtual void InitializeSelectionWidget() = 0;

	FORCEINLINE APlayerController* GetOwningController() const { return Cast<APlayerController>(GetOwner()); }

	UPROPERTY()
	TObjectPtr<ADietPlayerState> CachedPS;

	UPROPERTY(EditDefaultsOnly, Category = "Augment")
	TSubclassOf<UUserWidget> SelectionWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> ActiveWidgetInstance;

	TArray<TTuple<FName, int32>> CachedCandidates;
	int32 PendingLevelUpCount = 0;
	bool bIsSelecting = false;
};
