#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TemplateAugmentSelectionBase.generated.h"

class UHorizontalBox;
class UTemplateAugmentCardBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAugmentChosenSignature, FName, ChosenAugmentFName);

UCLASS()
class DIETSURVIVAL_API UTemplateAugmentSelectionBase : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeCards(const TArray<TTuple<FName, int32>>& Augments);

	void ClearContainer();

	UPROPERTY(BlueprintAssignable, Category = "Augment")
	FOnAugmentChosenSignature OnAugmentChosen;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleCardClicked(FName AugmentFName);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> CardContainer;

	UPROPERTY(EditDefaultsOnly, Category = "Augment")
	TSubclassOf<UTemplateAugmentCardBase> CardWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Augment")
	FMargin CardPadding = FMargin(40.f, 0.f);

private:
	UPROPERTY()
	TArray<TObjectPtr<UTemplateAugmentCardBase>> ActiveCards;
};
