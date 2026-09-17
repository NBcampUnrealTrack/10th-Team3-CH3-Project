#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

class USkillBase;

// 슬롯에 스킬이 새로 들어오거나 레벨업됐을 때 (UI 구독용)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillSlotChanged, int32, SlotIndex, USkillBase*, Skill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPassiveSlotChanged, int32, SlotIndex, USkillBase*, Skill);

// 선택된 슬롯이 바뀌었을 때 (UI 하이라이트용)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectedSlotChanged, int32, NewSlotIndex);

// 플레이어가 보유한 스킬들을 관리하는 컴포넌트
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIETSURVIVAL_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillComponent();

	// 스킬 슬롯 개수
	static constexpr int32 MaxSkillSlots = 4;
	static constexpr int32 MaxPassiveSlots = 4;

protected:
	virtual void BeginPlay() override;

public:
	// 스킬 변경 인스턴스
	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FOnSkillSlotChanged OnSkillSlotChanged;
	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FOnPassiveSlotChanged OnPassiveSlotChanged;
	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FOnSelectedSlotChanged OnSelectedSlotChanged;

	// 증강에 나올 수 있는 모든 스킬 후보 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<USkillBase>> AllPossibleSkillClasses;

	// 스킬 인스턴스 배열
	UPROPERTY(BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<USkillBase>> OwnedSkills;

	// 액티브 스킬 슬롯. 항상 길이 MaxSkillSlots로 고정되고, 빈 슬롯은 nullptr
	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	TArray<TObjectPtr<USkillBase>> SkillSlots;
	// 패시브 스킬 슬롯. 길이 고정, 빈 칸은 nullptr. 휠/클릭 대상은 아니고 UI 표시용
	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	TArray<TObjectPtr<USkillBase>> PassiveSlots;

	// 스킬을 획득하거나 이미 가지고 있는 스킬이면 레벨업
	UFUNCTION(BlueprintCallable, Category = "Skill")
	USkillBase* AcquireOrUpgradeSkill(TSubclassOf<USkillBase> SkillClass);

	// 현재 선택된 슬롯 번호 (0-based)
	UFUNCTION(BlueprintPure, Category = "Skill")
	int32 GetSelectedSlotIndex() const { return SelectedSlotIndex; }
	UFUNCTION(BlueprintPure, Category = "Skill")
	USkillBase* GetPassiveInSlot(int32 SlotIndex) const;

	// 슬롯 직접 선택 (숫자키 1~4용)
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetSelectedSlot(int32 NewIndex);

	// 휠 입력용. Direction이 양수면 다음 슬롯, 음수면 이전 슬롯 (끝에서 순환)
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void CycleSelectedSlot(int32 Direction);

	// 현재 선택된 슬롯의 스킬 발동 (좌클릭)
	UFUNCTION(BlueprintCallable, Category = "Skill")
	bool TryActivateSelectedSlot();

	// 보유 스킬 슬롯(OwnedSkills의 인덱스)으로 발동 시도. 아직 못 배운 스킬은 애초에 배열에 없어서 발동 불가 
	UFUNCTION(BlueprintCallable, Category = "Skill")
	bool TryActivateSkill(int32 SkillIndex);

	// 슬롯의 스킬 인스턴스 (쿨타임/레벨 UI 표시 등)
	UFUNCTION(BlueprintPure, Category = "Skill")
	USkillBase* GetSkillInSlot(int32 SlotIndex) const;

	// 해당 슬롯이 비어있는지
	UFUNCTION(BlueprintPure, Category = "Skill")
	bool IsSlotEmpty(int32 SlotIndex) const;

private:
	// 비어있는 가장 앞쪽 슬롯 인덱스. 없으면 INDEX_NONE
	int32 FindFirstEmptySlot() const;
	int32 FindFirstEmptyPassiveSlot() const;

	int32 SelectedSlotIndex = 0;
};
