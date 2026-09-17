#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillSelectionComponent.generated.h"

class ADietPlayerState;
class USkillSelectionWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIETSURVIVAL_API USkillSelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillSelectionComponent();

	// 테스트를 위해 public으로. 테스트 끝나면 protected로 옮겨야 함.
	UFUNCTION()
	void HandleSkillUp();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	UFUNCTION()
	void HandleSkillChosen(FName ChosenSkillFName);

	UFUNCTION()
	void TryBindToSkillUp();

private:
	void StartSelection();
	void FinishSelection();

	FORCEINLINE APlayerController* GetOwningController() const { return Cast<APlayerController>(GetOwner()); }

	UPROPERTY()
	TObjectPtr<ADietPlayerState> CachedPS;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<USkillSelectionWidget> SelectionWidgetClass;

	UPROPERTY()
	TObjectPtr<USkillSelectionWidget> ActiveWidgetInstance;

	TArray<TTuple<FName, int32>> CachedCandidates;
	int32 PendingSkillUpCount = 0;
	bool bIsSelecting = false;
};
