#include "Player/PlayerCharacterController.h"
#include "System/AugmentSelectionComponent.h"
#include "System/SkillSelectionComponent.h"

#include "System/DietPlayerState.h" // 버그 확인용

APlayerCharacterController::APlayerCharacterController()
{
	AugmentSelectionComponent = CreateDefaultSubobject<UAugmentSelectionComponent>(TEXT("AugmentSelection"));
	SkillSelectionComponent = CreateDefaultSubobject<USkillSelectionComponent>(TEXT("SkillSelection"));
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
	// 입력(IMC/IA) 관련 로직은 PlayerCharacter
}

void APlayerCharacterController::ExecBroadCastAugments()
{
	if (ADietPlayerState* PS = GetPlayerState<ADietPlayerState>())
	{
		PS->OnLevelUp.Broadcast(PS->GetCurrentLevel());
		PS->OnSkillUp.Broadcast();
	}
}
