#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExpWidget.generated.h"

class ADietPlayerState;

UCLASS()
class DIETSURVIVAL_API UExpWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void HandleLevelUp(int32 NewLevel);

	UFUNCTION()
	void HandleExpChanged(int32 CurrentExp, int32 MaxExp);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void OnLevelDisplayUpdated(int32 NewLevel);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void OnExpDisplayUpdated(float Ratio, int32 CurrentExp, int32 MaxExp);

private:
	UPROPERTY()
	TObjectPtr<ADietPlayerState> CachedPS;
};
