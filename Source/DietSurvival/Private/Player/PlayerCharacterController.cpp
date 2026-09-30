#include "Player/PlayerCharacterController.h"
#include "System/AugmentSelectionComponent.h"
#include "System/SkillSelectionComponent.h"
#include "System/DietPlayerState.h"

APlayerCharacterController::APlayerCharacterController()
{
	PrimaryActorTick.bCanEverTick = true;
	AugmentSelectionComponent = CreateDefaultSubobject<UAugmentSelectionComponent>(TEXT("AugmentSelection"));
	SkillSelectionComponent = CreateDefaultSubobject<USkillSelectionComponent>(TEXT("SkillSelection"));
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
	// 입력(IMC/IA) 관련 로직은 PlayerCharacter

	GetWorldTimerManager().SetTimerForNextTick(this, &APlayerCharacterController::TryBindToDelegate);
}

void APlayerCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PendingStatAugmentCount > 0 && !bIsSelectingAugment)
	{
		PendingStatAugmentCount--;
		AugmentSelectionComponent->StartSelection();
	}
	if (PendingSkillAugmentCount > 0 && !bIsSelectingAugment)
	{
		PendingSkillAugmentCount--;
		SkillSelectionComponent->StartSelection();
	}
}

void APlayerCharacterController::HandleLevelUp(int32 Level)
{
	PendingStatAugmentCount++;
}

void APlayerCharacterController::HandleSkillUp()
{
	PendingSkillAugmentCount++;
}

void APlayerCharacterController::ExecBroadCastAugments()
{
	if (ADietPlayerState* PS = GetPlayerState<ADietPlayerState>())
	{
		PS->OnLevelUp.Broadcast(PS->GetCurrentLevel());
		PS->OnSkillUp.Broadcast();
	}
}

void APlayerCharacterController::TryBindToDelegate()
{
	if (ADietPlayerState* PS = GetPlayerState<ADietPlayerState>())
	{
		PS->OnLevelUp.AddDynamic(this, &APlayerCharacterController::HandleLevelUp);
		PS->OnSkillUp.AddDynamic(this, &APlayerCharacterController::HandleSkillUp);
	}
}
