#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillSelectionWidget.generated.h"

class UHorizontalBox;
class USkillCardWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillChosenSignature, FName, ChosenSkillFName);

UCLASS()
class DIETSURVIVAL_API USkillSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeCards(const TArray<TTuple<FName, int32>>& Augments);

	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FOnSkillChosenSignature OnSkillChosen;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleCardClicked(FName SkillFName);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> CardContainer;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<USkillCardWidget> CardWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	FMargin CardPadding = FMargin(40.f, 0.f);

private:
	UPROPERTY()
	TArray<TObjectPtr<USkillCardWidget>> ActiveCards;
};
