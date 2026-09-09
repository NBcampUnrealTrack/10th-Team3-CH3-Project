#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class UUserHUDWidget;
class UPauseMenuWidget;
class ADietPlayerState;
class UPlayerStatComponent;

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void ShowMainHUD();
	UUserHUDWidget* GetHUDWidget() const { return UserHUDWidget; }

	UFUNCTION(Exec)
	void ShowPauseMenu();

	UFUNCTION(Exec)
	void HidePauseMenu();

protected:
	void SetUIInputMode(bool bUIOnly);

	void BindDelegates();

	UFUNCTION()
	void HandleExpChanged(int32 CurrentExp, int32 MaxExp);

	UFUNCTION()
	void HandleLevelUp(int32 NewLevel);

	UFUNCTION()
	void HandleFullnessChanged(float NewFullness, float MaxFullness);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserHUDWidget> UserHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserHUDWidget> UserHUDWidget;

	UPROPERTY()
	TObjectPtr<UPauseMenuWidget> PauseMenuWidget;

	UPROPERTY()
	TObjectPtr<ADietPlayerState> CachedPlayerState;

	UPROPERTY()
	TObjectPtr<UPlayerStatComponent> CachedStatComp;
};
