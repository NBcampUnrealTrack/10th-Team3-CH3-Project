#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TemplateAugmentCardBase.generated.h"

class UButton;
class UTextBlock;
class UWidgetAnimation;
class UTemplateAugmentCardBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardClickedSignatureTest, FName, AugmentFName);
// 카드 연출이 끝났을 때. 해당 애니메이션이 없는 카드는 호출 즉시 알림
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardAnimFinished, UTemplateAugmentCardBase*, Card);

UCLASS()
class DIETSURVIVAL_API UTemplateAugmentCardBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void SetupCard(FName InAugmentFName, int32 InAugmentLevel);

	// 등장. AppearAnim이 Render Opacity 0→1을 맡음
	void PlayAppear();
	// 선택됨: 번쩍이며 사라짐
	void PlayChosen();
	// 선택 안 됨: 페이드아웃. DismissAnim이 없으면 즉시 숨김
	void PlayDismiss();

	FName GetAugmentFName() const { return AugmentFName; }

	UPROPERTY(BlueprintAssignable)
	FOnCardClickedSignatureTest OnCardClicked;

	FOnCardAnimFinished OnAppearFinished;
	FOnCardAnimFinished OnChosenFinished;

protected:
	virtual void NativeConstruct() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

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

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> AppearAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> ChosenAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> DismissAnim;

	FName AugmentFName;
};
