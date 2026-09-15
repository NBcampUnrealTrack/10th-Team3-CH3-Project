#include "Player/Skill/SkillComponent.h"
#include "Player/Skill/SkillBase.h"
#include "Player/Skill/ActiveSkillBase.h"
#include "Player/Skill/PassiveSkillBase.h"
#include "Player/PlayerCharacter.h"

USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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
}

USkillBase* USkillComponent::AcquireOrUpgradeSkill(TSubclassOf<USkillBase> SkillClass)
{
	if (!SkillClass)
	{
		return nullptr;
	}

	// 이미 보유 중인 스킬인지 먼저 확인 (같은 클래스가 이미 OwnedSkills에 있는지)
	for (USkillBase* Owned : OwnedSkills)
	{
		if (Owned && Owned->GetClass() == SkillClass)
		{
			// 이미 갖고 있으면 새로 만들지 않고 레벨업만 시킴
			Owned->LevelUpSkill();
			return Owned;
		}
	}

	// 처음 고르는 스킬이면 이 시점에 인스턴스를 생성
	APlayerCharacter* OwnerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return nullptr;
	}

	USkillBase* NewSkill = NewObject<USkillBase>(this, SkillClass);
	if (NewSkill)
	{
		NewSkill->InitializeSkill(OwnerCharacter);
		OwnedSkills.Add(NewSkill);

		// 패시브 스킬이면 최초 획득 시점에 OnAcquired()를 호출해서 지속 효과를 바로 시작시킴.
		// 액티브 스킬은 이 훅이 필요 없으므로 (UPassiveSkillBase가 아니면) 호출 안 함
		if (UPassiveSkillBase* Passive = Cast<UPassiveSkillBase>(NewSkill))
		{
			Passive->OnAcquired();
		}

		UE_LOG(LogTemp, Log, TEXT("[SkillComponent] 스킬 최초 획득: %s"), *NewSkill->GetSkillName().ToString());
	}

	return NewSkill;
}

bool USkillComponent::TryActivateSkill(int32 SkillIndex)
{
	if (!OwnedSkills.IsValidIndex(SkillIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("[SkillComponent] 잘못된 스킬 인덱스 또는 아직 획득 안 한 스킬: %d"), SkillIndex);
		return false;
	}

	// 패시브 스킬은 획득 즉시 계속 작동
	UActiveSkillBase* ActiveSkill = Cast<UActiveSkillBase>(OwnedSkills[SkillIndex]);
	if (!ActiveSkill)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SkillComponent] %s는 패시브 스킬이라 수동 발동 불가"),
			OwnedSkills[SkillIndex] ? *OwnedSkills[SkillIndex]->GetSkillName().ToString() : TEXT("Unknown"));
		return false;
	}

	const bool bSuccess = ActiveSkill->TryActivate();

	if (!bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("[SkillComponent] %s 발동 실패 (쿨타임 %.1f초 남음)"),
			*ActiveSkill->GetSkillName().ToString(), ActiveSkill->GetRemainingCooldown());
	}

	return bSuccess;
}

USkillBase* USkillComponent::GetOwnedSkill(int32 SkillIndex) const
{
	return OwnedSkills.IsValidIndex(SkillIndex) ? OwnedSkills[SkillIndex] : nullptr;
}
