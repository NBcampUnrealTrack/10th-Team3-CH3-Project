#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillSlotWidget.generated.h"

class UImage;
class UTextBlock;
class UTexture2D;

UCLASS()
class DIETSURVIVAL_API USkillSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// CooldownEndTime은 월드 시간(초). 쿨타임이 없으면 0
	void SetSkill(UTexture2D* Icon, int32 Level, double CooldownEndTime);
	void ClearSkill();
	void SetSelected(bool bSelected);

protected:
	virtual void NativeDestruct() override;

	UFUNCTION()
	void UpdateCooldown();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IconImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CooldownText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> SelectedFrame;

	// 쿨타임 중 색 변경
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	FLinearColor CooldownTint = FLinearColor(0.3f, 0.3f, 0.3f, 1.f);

private:
	double CooldownEndTime = 0.0;
	FTimerHandle CooldownTimer;
};
