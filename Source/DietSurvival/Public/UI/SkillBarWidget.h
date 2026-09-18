#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillBarWidget.generated.h"

class UHorizontalBox;
class USkillSlotWidget;
class USkillComponent;
class USkillBase;
class UTexture2D;

UCLASS()
class DIETSURVIVAL_API USkillBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UFUNCTION()
	void HandleSkillSlotChanged(int32 SlotIndex, USkillBase* Skill);

	UFUNCTION()
	void HandlePassiveSlotChanged(int32 SlotIndex, USkillBase* Skill);

	UFUNCTION()
	void HandleSelectedSlotChanged(int32 NewSlotIndex);

	void BindSkillComponent(APawn* Pawn);
	void UnbindSkillComponent();

	// 스킬이 처음 오면 칸을 만들어 Box에 붙이고, 이후엔 그 칸을 갱신. Skill이 null이면 칸 제거
	void UpdateSlot(UHorizontalBox* Box, TArray<TObjectPtr<USkillSlotWidget>>& Slots, int32 SlotIndex, USkillBase* Skill);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> PassiveBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> ActiveBox;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<USkillSlotWidget> SlotWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	FMargin SlotPadding = FMargin(4.f, 0.f);

	// 키는 DT_SkillData의 RowName. USkillBase::GetSkillName()과 같아야 아이콘이 나옴
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TMap<FName, TSoftObjectPtr<UTexture2D>> SkillIcons;

private:
	// 인덱스 = SkillComponent 슬롯 번호. 아직 스킬이 없는 칸은 nullptr
	UPROPERTY()
	TArray<TObjectPtr<USkillSlotWidget>> ActiveSlotWidgets;

	UPROPERTY()
	TArray<TObjectPtr<USkillSlotWidget>> PassiveSlotWidgets;

	UPROPERTY()
	TObjectPtr<USkillComponent> CachedSkillComp;
};
