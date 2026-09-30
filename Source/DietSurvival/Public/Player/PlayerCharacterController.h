#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

class UAugmentSelectionComponent;
class USkillSelectionComponent;

UCLASS()
class DIETSURVIVAL_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerCharacterController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleLevelUp(int32 Level);

	UFUNCTION()
	void HandleSkillUp();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Diet")
	TObjectPtr<UAugmentSelectionComponent> AugmentSelectionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Diet")
	TObjectPtr<USkillSelectionComponent> SkillSelectionComponent;

	bool bIsSelectingAugment = false;

	// 동시에 levelup, skillup을 broadcast.
	UFUNCTION(Exec)
	void ExecBroadCastAugments();

private:
	void TryBindToDelegate();

private:
	int32 PendingStatAugmentCount = 0;
	int32 PendingSkillAugmentCount = 0;
};
