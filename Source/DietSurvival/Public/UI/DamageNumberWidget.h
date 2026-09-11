#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNumberWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class DIETSURVIVAL_API UDamageNumberWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetDamage(float Damage);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DamageText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FloatUpAnim;
};
