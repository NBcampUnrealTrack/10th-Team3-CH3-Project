#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TemplateAugmentCardBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardClickedSignatureTest, FName, AugmentFName);

class UButton;
class UTextBlock;

UCLASS()
class DIETSURVIVAL_API UTemplateAugmentCardBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void SetupCard(FName InAugmentFName, int32 InAugmentLevel);

	UPROPERTY(BlueprintAssignable)
	FOnCardClickedSignatureTest OnCardClicked;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleButtonClicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "Diet")
	void OnCardDataReady(const FText& AugmentUIName, const FText& Description);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CardButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DescriptionText;

	FName AugmentFName;
	
};
