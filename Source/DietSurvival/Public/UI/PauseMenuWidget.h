#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UButton;

UCLASS()
class DIETSURVIVAL_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleResumeClicked();

	UFUNCTION()
	void HandleQuitClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ResumeButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;
};
