#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AugmentCardWidget.generated.h"

class UButton;

UCLASS()
class DIETSURVIVAL_API UAugmentCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupCard(FName InAugmentFName, int32 InAugmentLevel);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardClickedSignature, FName, AugmentFName);

	UPROPERTY(BlueprintAssignable, Category = "Augment")
	FOnCardClickedSignature OnCardClicked;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleButtonClicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "Augment")
	void OnCardDataReady(const FText& AugmentName, const FText& Description);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CardButton;

private:
	FName AugmentFName;
};
