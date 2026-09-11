#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class UUserHUDWidget;
class UPauseMenuWidget;
class ADietPlayerState;
class UPlayerStatComponent;
class UAugmentSelectionWidget;
class UResultWidget;

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

	UFUNCTION(Exec, BlueprintCallable, Category = "UI")
	void ShowPauseMenu();

	UFUNCTION(Exec, BlueprintCallable, Category = "UI")
	void HidePauseMenu();

	UAugmentSelectionWidget* ShowAugmentSelect(const TArray<TTuple<FName, int32>>& Augments);

	void HideAugmentSelect();

	void ShowResult(bool bWin);

	void HideResult();

	// 임시테스트용 함수들. 나중에 삭제 예정.
	UFUNCTION(Exec)
	void TestFullness(float Current, float Max);

	UFUNCTION(Exec)
	void TestTimer(float Seconds);

	UFUNCTION(Exec)
	void TestHitMarker();

	UFUNCTION(Exec)
	void TestResult(bool bWin);
	// 요기까지

protected:
	void SetUIInputMode(bool bUIOnly);

	void BindDelegates();

	UFUNCTION()
	void HandleExpChanged(float CurrentExp, float MaxExp);

	UFUNCTION()
	void HandleLevelUp(int32 NewLevel);

	UFUNCTION()
	void HandleFullnessChanged(float NewFullness, float MaxFullness);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserHUDWidget> UserHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UAugmentSelectionWidget> AugmentSelectWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UResultWidget> ResultWidgetClass;

	UPROPERTY()
	TObjectPtr<UAugmentSelectionWidget> AugmentSelectWidget;

	UPROPERTY()
	TObjectPtr<UUserHUDWidget> UserHUDWidget;

	UPROPERTY()
	TObjectPtr<UPauseMenuWidget> PauseMenuWidget;

	UPROPERTY()
	TObjectPtr<UResultWidget> ResultWidget;

	UPROPERTY()
	TObjectPtr<ADietPlayerState> CachedPlayerState;

	UPROPERTY()
	TObjectPtr<UPlayerStatComponent> CachedStatComp;

};
