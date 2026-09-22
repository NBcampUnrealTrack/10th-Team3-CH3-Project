#include "Player/Skill/SkillBase.h"
#include "Player/PlayerCharacter.h"

void USkillBase::InitializeSkill(APlayerCharacter* InOwner, FName InSkillFName)
{
	OwnerCharacter = InOwner;
	SkillName = InSkillFName;
}

void USkillBase::LevelUpSkill(const FSkillDeltaRow& DeltaRow)
{
	++Level;

	// 실제 레벨업 시 처리 로직은 자식 클래스에서 구현
	OnLevelUp(DeltaRow);
}
