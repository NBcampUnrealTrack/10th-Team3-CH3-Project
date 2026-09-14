#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UImage;
class UWidgetAnimation;
class UMaterialInstanceDynamic;

UCLASS()
class DIETSURVIVAL_API UUserHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetFullness(float CurrentFullness, float MaxFullness);
	void SetExp(float CurrentExp, float MaxExp);
	void SetLevel(int32 Level);
	void SetTimer(float ElapsedTime);
	void SetAmmo(int32 CurrentAmmo, int32 MaxAmmo);
	void SetKillCount(int32 Count);

	void PlayHitMarker();
	void PlayKillConfirm();
	void PlayHitFlash();
	void SetFullnessWarning(float Ratio);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> FullnessBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ExpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimerText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AmmoText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KillCountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> HitFlashVignette;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HitMarkerAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> KillConfirmAnim;

	// 피격 번쩍임이 사라지는 속도. 클수록 빨리 꺼짐
	UPROPERTY(EditDefaultsOnly, Category = "Vignette")
	float HitFlashDecay = 8.f;

	// 포만감 0% / 100% 일 때 피격 비네트 세기
	UPROPERTY(EditDefaultsOnly, Category = "Vignette")
	float HitFlashMinIntensity = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Vignette")
	float HitFlashMaxIntensity = 1.f;

private:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> HitFlashMID;

	float HitFlashIntensity = 0.f;
	float FullnessRatio = 0.f;
};
