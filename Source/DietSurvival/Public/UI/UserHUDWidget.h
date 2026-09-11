#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UBorder;
class UWidgetAnimation;

UCLASS()
class DIETSURVIVAL_API UUserHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetFullness(float CurrentFullness, float MaxFullness);
	void SetExp(float CurrentExp, float MaxExp);
	void SetLevel(int32 Level);
	void SetTimer(float ElapsedTime);
	void SetWave(int32 Wave);
	void SetAmmo(int32 CurrentAmmo, int32 MaxAmmo);
	void SetKillCount(int32 Count);

	void PlayHitMarker();
	void PlayKillConfirm();
	void PlayHitFlash();
	void SetFullnessWarning(bool bShow);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> FullnessBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ExpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimerText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WaveText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AmmoText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KillCountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> FullnessWarningBorder;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HitMarkerAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> KillConfirmAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HitFlashAnim;
};
