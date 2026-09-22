#include "Player/Skill/SkillComponent.h"
#include "Player/Skill/SkillBase.h"
#include "Player/Skill/ActiveSkillBase.h"
#include "Player/Skill/PassiveSkillBase.h"
#include "Player/PlayerCharacter.h"

#include "System/DataTableSubsystem.h"
#include "System/SkillDataRow.h"

USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SkillSlots.SetNum(MaxSkillSlots);
	PassiveSlots.SetNum(MaxPassiveSlots);
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* OwnerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SkillComponent] Owner가 PlayerCharacter가 아님"));
		return;
	}

	// 초기 상태 브로드캐스트
	OnSelectedSlotChanged.Broadcast(SelectedSlotIndex);
}

USkillBase* USkillComponent::AcquireOrUpgradeSkill(FName SkillFName, int32 SkillLevel)
{
	UDataTableSubsystem* DataTableSubsystem = UDataTableSubsystem::Get(this);

	//DT 미등록 시
	if (!DataTableSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SkillComponent] DataTableSubsystem을 찾을 수 없음"));
		return nullptr;
	}

	// 스킬 클래스 가져옴
	TSubclassOf<USkillBase> SkillClass = DataTableSubsystem->GetSkillClass(SkillFName);
	if (!SkillClass)
	{
		return nullptr;
	}

	// 스킬 이름과 레벨에 맞는 데이터 가져옴
	const FSkillDeltaRow& DeltaRow = DataTableSubsystem->GetSkillDeltaRow(SkillFName, SkillLevel);

	// 이미 보유 중이면 새로 만들지 않고 레벨업만
	for (USkillBase* Owned : OwnedSkills)
	{
		if (Owned && Owned->GetClass() == SkillClass)
		{
			Owned->LevelUpSkill(DeltaRow);

			// 레벨 표시 갱신을 위해 어느 슬롯에 있는지 찾아서 알림
			const int32 ActiveIndex = SkillSlots.IndexOfByKey(Owned);
			if (ActiveIndex != INDEX_NONE)
			{
				OnSkillSlotChanged.Broadcast(ActiveIndex, Owned);
			}
			else
			{
				const int32 PassiveIndex = PassiveSlots.IndexOfByKey(Owned);
				if (PassiveIndex != INDEX_NONE)
				{
					OnPassiveSlotChanged.Broadcast(PassiveIndex, Owned);
				}
			}
			return Owned;
		}
	}

	APlayerCharacter* OwnerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return nullptr;
	}

	const bool bIsActiveSkill = SkillClass->IsChildOf(UActiveSkillBase::StaticClass());
	const int32 TargetSlot = bIsActiveSkill ? FindFirstEmptySlot() : FindFirstEmptyPassiveSlot();

	// 해당 종류 슬롯이 꽉 찼으면 획득 자체를 취소
	if (TargetSlot == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SkillComponent] %s 슬롯이 가득 참. 획득 취소: %s"),
			bIsActiveSkill ? TEXT("액티브") : TEXT("패시브"), *SkillClass->GetName());
		return nullptr;
	}

	USkillBase* NewSkill = NewObject<USkillBase>(this, SkillClass);
	if (!NewSkill)
	{
		return nullptr;
	}

	NewSkill->InitializeSkill(OwnerCharacter, SkillFName);
	OwnedSkills.Add(NewSkill);

	NewSkill->OnAcquired(DeltaRow);

	if (bIsActiveSkill)
	{
		SkillSlots[TargetSlot] = NewSkill;
		OnSkillSlotChanged.Broadcast(TargetSlot, NewSkill);
	}
	else
	{
		PassiveSlots[TargetSlot] = NewSkill;
		OnPassiveSlotChanged.Broadcast(TargetSlot, NewSkill);
	}

	UE_LOG(LogTemp, Log, TEXT("[SkillComponent] %s 스킬 최초 획득: %s (슬롯 %d)"),
		bIsActiveSkill ? TEXT("액티브") : TEXT("패시브"),
		*NewSkill->GetSkillName().ToString(), TargetSlot + 1);

	return NewSkill;
}

void USkillComponent::SetSelectedSlot(int32 NewIndex)
{
	if (!SkillSlots.IsValidIndex(NewIndex) || NewIndex == SelectedSlotIndex)
	{
		return;
	}

	SelectedSlotIndex = NewIndex;
	OnSelectedSlotChanged.Broadcast(SelectedSlotIndex);
}

void USkillComponent::CycleSelectedSlot(int32 Direction)
{
	if (Direction == 0 || MaxSkillSlots <= 0)
	{
		return;
	}

	// 채워진 슬롯 인덱스만 모음
	TArray<int32> FilledIndices;
	for (int32 i = 0; i < SkillSlots.Num(); ++i)
	{
		if (SkillSlots[i] != nullptr)
		{
			FilledIndices.Add(i);
		}
	}

	// 스킬이 하나도 없으면 휠을 돌려도 아무 일 없음
	if (FilledIndices.Num() == 0)
	{
		return;
	}

	// 스킬이 1개뿐이면 그 슬롯에 고정
	if (FilledIndices.Num() == 1)
	{
		SetSelectedSlot(FilledIndices[0]);
		return;
	}

	// 현재 선택된 슬롯이 채워진 목록에서 몇 번째인지 찾음
	int32 CurrentPos = FilledIndices.IndexOfByKey(SelectedSlotIndex);

	// 선택된 슬롯이 마침 빈 슬롯이었다면(초기 상태 등) 첫 채워진 슬롯 기준으로 시작
	if (CurrentPos == INDEX_NONE)
	{
		CurrentPos = 0;
	}

	const int32 Step = (Direction > 0) ? 1 : -1; // 마우스 휠 올리면 1, 내리면 -1
	const int32 NewPos = (CurrentPos + Step + FilledIndices.Num()) % FilledIndices.Num(); //항상 양수를 % 연산하기 위해 + FilledIndices.Num()

	SetSelectedSlot(FilledIndices[NewPos]);
}

bool USkillComponent::TryActivateSelectedSlot()
{
	return TryActivateSkill(SelectedSlotIndex);
}

bool USkillComponent::TryActivateSkill(int32 SlotIndex)
{
	if (!SkillSlots.IsValidIndex(SlotIndex))
	{
		return false;
	}

	// 빈 슬롯에서 클릭한 경우는 조용히 실패 (로그 도배 방지)
	UActiveSkillBase* ActiveSkill = Cast<UActiveSkillBase>(SkillSlots[SlotIndex]);
	if (!ActiveSkill)
	{
		return false;
	}

	const bool bSuccess = ActiveSkill->TryActivate();
	if (bSuccess)
	{
		OnSkillSlotChanged.Broadcast(SlotIndex, ActiveSkill); // UI 갱신용
	}
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("[SkillComponent] %s 발동 실패 (쿨타임 %.1f초 남음)"),
			*ActiveSkill->GetSkillName().ToString(), ActiveSkill->GetRemainingCooldown());
		
	}

	return bSuccess;
}

USkillBase* USkillComponent::GetSkillInSlot(int32 SlotIndex) const
{
	return SkillSlots.IsValidIndex(SlotIndex) ? SkillSlots[SlotIndex] : nullptr;
}

USkillBase* USkillComponent::GetPassiveInSlot(int32 SlotIndex) const
{
	return PassiveSlots.IsValidIndex(SlotIndex) ? PassiveSlots[SlotIndex] : nullptr;
}

bool USkillComponent::IsSlotEmpty(int32 SlotIndex) const
{
	return GetSkillInSlot(SlotIndex) == nullptr;
}

int32 USkillComponent::FindFirstEmptySlot() const
{
	for (int32 i = 0; i < SkillSlots.Num(); ++i)
	{
		if (SkillSlots[i] == nullptr)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

int32 USkillComponent::FindFirstEmptyPassiveSlot() const
{
	for (int32 i = 0; i < PassiveSlots.Num(); ++i)
	{
		if (PassiveSlots[i] == nullptr)
		{
			return i;
		}
	}
	return INDEX_NONE;
}
