#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillSelectionComponent.generated.h"

class ADietPlayerState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIETSURVIVAL_API USkillSelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillSelectionComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	UFUNCTION(exec)
	void HandleSkillUp();

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

	TArray<TTuple<FName, int32>> CachedCandidates;
	int32 PendingSkillUpCount = 0;
	bool bIsSelecting = false;
};
