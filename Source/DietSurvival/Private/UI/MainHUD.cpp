#include "UI/MainHUD.h"
#include "UI/UserHUDWidget.h"
#include "UI/PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "System/DietPlayerState.h"
#include "Player/PlayerStatComponent.h"
#include "GameFramework/Pawn.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	ShowMainHUD();

	GetWorldTimerManager().SetTimerForNextTick(this, &AMainHUD::BindDelegates);
}

void AMainHUD::ShowMainHUD()
{
	if (UserHUDWidget || !UserHUDWidgetClass)
	{
		return;
	}

	UserHUDWidget = CreateWidget<UUserHUDWidget>(GetOwningPlayerController(), UserHUDWidgetClass);
	if (!UserHUDWidget)
	{
		return;
	}

	UserHUDWidget->AddToViewport(static_cast<int32>(EUILayer::HUD));

	// 델리게이트 연결되면 삭제
	UserHUDWidget->SetTimer(949.f);
	UserHUDWidget->SetAmmo(24, 30);
	UserHUDWidget->SetKillCount(99);
}

void AMainHUD::ShowPauseMenu()
{
	if (PauseMenuWidget || !PauseMenuWidgetClass)
	{
		return;
	}

	PauseMenuWidget = CreateWidget<UPauseMenuWidget>(GetOwningPlayerController(), PauseMenuWidgetClass);
	if (!PauseMenuWidget)
	{
		return;
	}

	PauseMenuWidget->AddToViewport(static_cast<int32>(EUILayer::PauseMenu));
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	SetUIInputMode(true);
}

void AMainHUD::HidePauseMenu()
{
	if (!PauseMenuWidget)
	{
		return;
	}

	PauseMenuWidget->RemoveFromParent();
	PauseMenuWidget = nullptr;

	UGameplayStatics::SetGamePaused(GetWorld(), false);
	SetUIInputMode(false);
}

void AMainHUD::SetUIInputMode(bool bUIOnly)
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC)
	{
		return;
	}

	if (bUIOnly)
	{
		PC->SetInputMode(FInputModeUIOnly());
		PC->SetShowMouseCursor(true);
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}
}

void AMainHUD::BindDelegates()
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC)
	{
		return;
	}

	CachedPlayerState = PC->GetPlayerState<ADietPlayerState>();
	if (CachedPlayerState)
	{
		CachedPlayerState->OnExpChanged.AddDynamic(this, &AMainHUD::HandleExpChanged);
		CachedPlayerState->OnLevelUp.AddDynamic(this, &AMainHUD::HandleLevelUp);

		HandleExpChanged(CachedPlayerState->GetCurrentExp(), CachedPlayerState->GetMaxExp());
		HandleLevelUp(CachedPlayerState->GetCurrentLevel());
	}

	APawn* Pawn = PC->GetPawn();
	CachedStatComp = Pawn ? Pawn->FindComponentByClass<UPlayerStatComponent>() : nullptr;
	if (CachedStatComp)
	{
		CachedStatComp->OnFullnessChanged.AddDynamic(this, &AMainHUD::HandleFullnessChanged);

		HandleFullnessChanged(CachedStatComp->GetFullness(), CachedStatComp->GetMaxFullness());
	}
}

void AMainHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CachedPlayerState)
	{
		CachedPlayerState->OnExpChanged.RemoveDynamic(this, &AMainHUD::HandleExpChanged);
		CachedPlayerState->OnLevelUp.RemoveDynamic(this, &AMainHUD::HandleLevelUp);
	}

	if (CachedStatComp)
	{
		CachedStatComp->OnFullnessChanged.RemoveDynamic(this, &AMainHUD::HandleFullnessChanged);
	}

	Super::EndPlay(EndPlayReason);
}

void AMainHUD::HandleExpChanged(int32 CurrentExp, int32 MaxExp)
{
	UserHUDWidget->SetExp(CurrentExp, MaxExp);
}

void AMainHUD::HandleLevelUp(int32 NewLevel)
{
	UserHUDWidget->SetLevel(NewLevel);
}

void AMainHUD::HandleFullnessChanged(float NewFullness, float MaxFullness)
{
	UserHUDWidget->SetFullness(NewFullness, MaxFullness);
}
