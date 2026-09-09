#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AugmentSelectionComponent.generated.h"

class ADietPlayerState;
class UAugmentSelectionWidget;

UCLASS( ClassGroup=(DietSurvival), meta=(BlueprintSpawnableComponent) )
class DIETSURVIVAL_API UAugmentSelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAugmentSelectionComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	UFUNCTION()
	void HandleLevelUp(int32 NewLevel);

	UFUNCTION()
	void HandleAugmentChosen(FName ChosenAugmentFName);

	UFUNCTION()
	void TryBindToLevelUp();

private:
	void StartSelection();
	void FinishSelection();

	FORCEINLINE APlayerController* GetOwningController() const { return Cast<APlayerController>(GetOwner()); }

	UPROPERTY()
	TObjectPtr<ADietPlayerState> CachedPS;

	UPROPERTY(EditDefaultsOnly, Category = "Augment")
	TSubclassOf<UAugmentSelectionWidget> SelectionWidgetClass;

	UPROPERTY()
	TObjectPtr<UAugmentSelectionWidget> ActiveWidgetInstance;

	int32 PendingLevelUpCount = 0;
	bool bIsSelecting = false;
};
