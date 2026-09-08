#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class UUserHUDWidget;
class UPauseMenuWidget;

UENUM()
enum class EUILayer : uint8
{
	HUD = 0,
	DamageNumber = 50,
	AugmentSelect = 100,
	PauseMenu = 200,
};

UCLASS()
class DIETSURVIVAL_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void ShowMainHUD();
	UUserHUDWidget* GetHUDWidget() const { return UserHUDWidget; }

	void ShowPauseMenu();
	void HidePauseMenu();

protected:
	void SetUIInputMode(bool bUIOnly);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserHUDWidget> UserHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserHUDWidget> UserHUDWidget;

	UPROPERTY()
	TObjectPtr<UPauseMenuWidget> PauseMenuWidget;
};
