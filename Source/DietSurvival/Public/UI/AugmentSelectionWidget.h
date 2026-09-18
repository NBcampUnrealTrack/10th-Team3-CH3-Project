#pragma once

#include "UI/TemplateAugmentSelectionBase.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AugmentSelectionWidget.generated.h"

class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRerollPressedSignature);

UCLASS()
class DIETSURVIVAL_API UAugmentSelectionWidget : public UTemplateAugmentSelectionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Augment")
	FOnRerollPressedSignature OnRerollPressed;

protected:
	UFUNCTION()
	void HandleRerollClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RerollButton;

	virtual void NativeConstruct() override;
};
