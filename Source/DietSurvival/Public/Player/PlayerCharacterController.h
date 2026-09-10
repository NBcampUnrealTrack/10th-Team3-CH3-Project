#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

class UAugmentSelectionComponent;

UCLASS()
class DIETSURVIVAL_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerCharacterController();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Augment")
	TObjectPtr<UAugmentSelectionComponent> AugmentSelectionComponent;
};
