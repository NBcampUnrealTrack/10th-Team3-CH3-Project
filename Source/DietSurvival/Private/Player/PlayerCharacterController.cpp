#include "Player/PlayerCharacterController.h"
#include "System/AugmentSelectionComponent.h"
#include "System/SkillSelectionComponent.h"

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

void APlayerCharacterController::ExecHandleSkillUp()
{
	SkillSelectionComponent->HandleSkillUp();
}
