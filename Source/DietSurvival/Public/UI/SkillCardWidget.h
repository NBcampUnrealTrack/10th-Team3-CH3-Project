#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillCardWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardClickedSignature, FName, SkillFName);

class UButton;
class UTextBlock;

UCLASS()
class DIETSURVIVAL_API USkillCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupCard(FName InSkillFName, int32 InSkillLevel);

	UPROPERTY(BlueprintAssignable)
	FOnCardClickedSignature OnCardClicked;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleButtonClicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "Diet")
	void OnCardDataReady(const FText& SkillName, const FText& Description);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CardButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DescriptionText;

private:
	FName SkillFName;
};
