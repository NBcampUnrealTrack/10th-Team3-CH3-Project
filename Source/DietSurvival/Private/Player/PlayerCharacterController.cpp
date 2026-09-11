#include "Player/PlayerCharacterController.h"
#include "System/AugmentSelectionComponent.h"

APlayerCharacterController::APlayerCharacterController()
{
	AugmentSelectionComponent = CreateDefaultSubobject<UAugmentSelectionComponent>(TEXT("AugmentSelection"));
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
	// 입력(IMC/IA) 관련 로직은 PlayerCharacter
}
