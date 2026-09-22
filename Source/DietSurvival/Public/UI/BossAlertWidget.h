#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossAlertWidget.generated.h"

class UWidgetAnimation;

UCLASS()
class DIETSURVIVAL_API UBossAlertWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ShowAnim;
};
