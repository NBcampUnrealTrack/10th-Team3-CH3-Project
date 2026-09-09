#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AugmentSelectionWidget.generated.h"

class UHorizontalBox;
class UAugmentCardWidget;

UCLASS()
class DIETSURVIVAL_API UAugmentSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeCards(const TArray<TTuple<FName, int32>>& Augments);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAugmentChosenSignature, FName, ChosenAugmentId);

	UPROPERTY(BlueprintAssignable, Category = "Augment")
	FOnAugmentChosenSignature OnAugmentChosen;

protected:
	UFUNCTION()
	void HandleCardClicked(FName AugmentFName);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> CardContainer;

	UPROPERTY(EditDefaultsOnly, Category = "Augment")
	TSubclassOf<UAugmentCardWidget> CardWidgetClass;

private:
	UPROPERTY()
	TArray<TObjectPtr<UAugmentCardWidget>> ActiveCards;
};
