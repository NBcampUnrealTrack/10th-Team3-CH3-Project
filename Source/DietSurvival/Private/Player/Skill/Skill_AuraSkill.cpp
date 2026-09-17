#include "Player/Skill/Skill_AuraSkill.h"
#include "Player/Skill/AuraOrbActor.h"
#include "Player/PlayerCharacter.h"

USkill_AuraSkill::USkill_AuraSkill()
{
	SkillName = TEXT("오라 스킬");
}

void USkill_AuraSkill::OnAcquired()
{
	// 최초 획득 시점(Level=1)에 필요한 개수만큼 오브를 스폰
	const int32 DesiredCount = CalculateDesiredOrbCount();
	for (int32 i = 0; i < DesiredCount; ++i)
	{
		SpawnOrb();
	}

	RedistributeOrbAngles();
	UpdateAllOrbDamage();
}

void USkill_AuraSkill::OnLevelUp()
{
	Super::OnLevelUp();

	// 데미지 배율은 매 레벨 항상 증가
	DamageMultiplier += DamageMultiplierIncreasePerLevel;

	// 오브 개수는 LevelsPerExtraOrb(기본 3) 레벨마다 하나씩만 늘어남
	const int32 DesiredCount = CalculateDesiredOrbCount();
	while (SpawnedOrbs.Num() < DesiredCount)
	{
		SpawnOrb();
	}

	// 오브 개수가 이번에 늘었을 수도, 안 늘었을 수도 있지만
	// 늘었을 경우를 대비해 항상 재배치 (안 늘었으면 각도 그대로 유지됨)
	RedistributeOrbAngles();
	UpdateAllOrbDamage();
}

int32 USkill_AuraSkill::CalculateDesiredOrbCount() const
{
	// 1레벨부터 오브 1개로 시작, LevelsPerExtraOrb(기본 3) 레벨마다 하나씩 추가됨
	if (LevelsPerExtraOrb <= 0)
	{
		return 1;
	}
	return 1 + (Level / LevelsPerExtraOrb);
}

void USkill_AuraSkill::SpawnOrb()
{
	APlayerCharacter* Owner = OwnerCharacter.Get();
	if (!Owner || !OrbActorClass)
	{
		return;
	}

	UWorld* World = Owner->GetWorld();
	if (!World)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;

	AAuraOrbActor* NewOrb = World->SpawnActor<AAuraOrbActor>(
		OrbActorClass, Owner->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);

	if (NewOrb)
	{
		// 각도는 RedistributeOrbAngles가 일괄로 다시 정해주므로 여기선 0으로 초기화만 해둠
		NewOrb->InitializeOrb(Owner, OrbitRadius, OrbitSpeed, 0.f);
		SpawnedOrbs.Add(NewOrb);
	}
}

void USkill_AuraSkill::RedistributeOrbAngles()
{
	const int32 OrbCount = SpawnedOrbs.Num();
	if (OrbCount == 0)
	{
		return;
	}

	// 오브 개수만큼 360도를 균등하게 나눠서 각자 다른 시작 각도를 줌
	const float AngleStep = 360.f / OrbCount;
	for (int32 i = 0; i < OrbCount; ++i)
	{
		if (AAuraOrbActor* Orb = SpawnedOrbs[i])
		{
			Orb->SetBaseAngleOffset(AngleStep * i);
		}
	}
}

void USkill_AuraSkill::UpdateAllOrbDamage()
{
	for (AAuraOrbActor* Orb : SpawnedOrbs)
	{
		if (Orb)
		{
			Orb->SetDamageMultiplier(DamageMultiplier);
		}
	}
}
