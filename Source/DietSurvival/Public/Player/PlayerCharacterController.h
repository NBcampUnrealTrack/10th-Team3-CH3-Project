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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Diet")
	TObjectPtr<UAugmentSelectionComponent> AugmentSelectionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Diet")
	TObjectPtr<USkillSelectionComponent> SkillSelectionComponent;

public:
	// Test
	UFUNCTION(Exec)
	void ExecHandleSkillUp();
};
