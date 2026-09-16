#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AugmentSelectionWidget.generated.h"

class UHorizontalBox;
class UAugmentCardWidget;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAugmentChosenSignature, FName, ChosenAugmentFName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRerollPressedSignature);

UCLASS()
class DIETSURVIVAL_API UAugmentSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeCards(const TArray<TTuple<FName, int32>>& Augments);

	void ClearContainer();

	UPROPERTY(BlueprintAssignable, Category = "Augment")
	FOnAugmentChosenSignature OnAugmentChosen;

	UPROPERTY(BlueprintAssignable, Category = "Augment")
	FOnRerollPressedSignature OnRerollPressed;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleCardClicked(FName AugmentFName);

	UFUNCTION()
	void HandleRerollClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> CardContainer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RerollButton;

	UPROPERTY(EditDefaultsOnly, Category = "Augment")
	TSubclassOf<UAugmentCardWidget> CardWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Augment")
	FMargin CardPadding = FMargin(40.f, 0.f);

private:
	UPROPERTY()
	TArray<TObjectPtr<UAugmentCardWidget>> ActiveCards;
};
