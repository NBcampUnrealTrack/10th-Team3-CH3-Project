#include "UI/MainHUD.h"
#include "UI/UserHUDWidget.h"
#include "UI/PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "System/DietPlayerState.h"
#include "Player/PlayerStatComponent.h"
#include "UI/AugmentSelectionWidget.h"
#include "UI/ResultWidget.h"
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
}

void AMainHUD::ShowPauseMenu()
{
	if (PauseMenuWidget || !PauseMenuWidgetClass || UGameplayStatics::IsGamePaused(GetWorld()))
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

UAugmentSelectionWidget* AMainHUD::ShowAugmentSelect(const TArray<TTuple<FName, int32>>& Augments)
{
	if (AugmentSelectWidget || !AugmentSelectWidgetClass)
	{
		return nullptr;
	}

	AugmentSelectWidget = CreateWidget<UAugmentSelectionWidget>(GetOwningPlayerController(), AugmentSelectWidgetClass);
	if (!AugmentSelectWidget)
	{
		return nullptr;
	}

	AugmentSelectWidget->InitializeCards(Augments);
	AugmentSelectWidget->AddToViewport(static_cast<int32>(EUILayer::AugmentSelect));

	UGameplayStatics::SetGamePaused(GetWorld(), true);
	SetUIInputMode(true);

	return AugmentSelectWidget;
}

void AMainHUD::HideAugmentSelect()
{
	if (!AugmentSelectWidget)
	{
		return;
	}

	AugmentSelectWidget->RemoveFromParent();
	AugmentSelectWidget = nullptr;

	UGameplayStatics::SetGamePaused(GetWorld(), false);
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		SetUIInputMode(false);
	}
}

void AMainHUD::ShowResult(bool bWin)
{
	if (ResultWidget || !ResultWidgetClass)
	{
		return;
	}

	ResultWidget = CreateWidget<UResultWidget>(GetOwningPlayerController(), ResultWidgetClass);
	if (!ResultWidget)
	{
		return;
	}

	ResultWidget->AddToViewport(static_cast<int32>(EUILayer::PauseMenu));
	ResultWidget->OnResultReady(bWin);
	SetUIInputMode(true);
}

void AMainHUD::HideResult()
{
	if (!ResultWidget)
	{
		return;
	}

	ResultWidget->RemoveFromParent();
	ResultWidget = nullptr;

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
	if (!PC || !UserHUDWidget)
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

void AMainHUD::HandleExpChanged(float CurrentExp, float MaxExp)
{
	if (!UserHUDWidget) { return; }
	UserHUDWidget->SetExp(CurrentExp, MaxExp);
}

void AMainHUD::HandleLevelUp(int32 NewLevel)
{
	if (!UserHUDWidget) { return; }
	UserHUDWidget->SetLevel(NewLevel);
}

void AMainHUD::HandleFullnessChanged(float NewFullness, float MaxFullness)
{
	if (!UserHUDWidget) { return; }
	UserHUDWidget->SetFullness(NewFullness, MaxFullness);
}

// 임시테스트용 함수들. 나중에 삭제 예정.
void AMainHUD::TestFullness(float Current, float Max)
{
	if (UserHUDWidget) { UserHUDWidget->SetFullness(Current, Max); }
}

void AMainHUD::TestTimer(float Seconds)
{
	if (UserHUDWidget) { UserHUDWidget->SetTimer(Seconds); }
}

void AMainHUD::TestHitMarker()
{
	if (UserHUDWidget) { UserHUDWidget->PlayHitMarker(); }
}

void AMainHUD::TestResult(bool bWin)
{
	ShowResult(bWin);
}
// 요기까지
