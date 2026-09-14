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
class ADietGameState;
class APlayerCharacter;
class UAttackComponent;

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

protected:
	void SetUIInputMode(bool bUIOnly);

	void BindDelegates();

	UFUNCTION()
	void HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	void BindPawnDelegates(APawn* Pawn);
	void UnbindPawnDelegates();

	UFUNCTION()
	void HandleExpChanged(float CurrentExp, float MaxExp);

	UFUNCTION()
	void HandleLevelUp(int32 NewLevel);

	UFUNCTION()
	void HandleFullnessChanged(float NewFullness, float MaxFullness);

	UFUNCTION()
	void HandleElapsedTimeUpdated(float ElapsedSeconds);

	UFUNCTION()
	void HandleInvincibilityChanged(bool bIsNowInvincible);

	UFUNCTION()
	void HandleAttackHit(AActor* HitActor, float DamageAmount);

	UFUNCTION()
	void HandleFullnessMax();

	UFUNCTION()
	void HandleTimeUp();

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

	UPROPERTY()
	TObjectPtr<ADietGameState> CachedGameState;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> CachedPlayerCharacter;

	UPROPERTY()
	TObjectPtr<UAttackComponent> CachedAttackComp;

};
