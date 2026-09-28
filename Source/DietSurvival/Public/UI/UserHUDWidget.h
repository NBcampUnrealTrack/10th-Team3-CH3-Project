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
	void SetAmmo(int32 CurrentAmmo);
	void SetKillCount(int32 Count);
	void StartReload();
	void FinishReload();
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
	TObjectPtr<UImage> ReloadBar;

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

	// 재장전 바가 차는 시간(초). AttackComponent의 장전 시간과 맞춤
	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	float ReloadDuration = 2.f;

private:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> HitFlashMID;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> ReloadMID;

	float HitFlashIntensity = 0.f;
	float FullnessRatio = 0.f;
	bool bReloading = false;
	float ReloadElapsed = 0.f;
};
