#include "UI/MainHUD.h"
#include "UI/UserHUDWidget.h"
#include "UI/PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "System/DietPlayerState.h"
#include "Player/PlayerStatComponent.h"
#include "UI/AugmentSelectionWidget.h"
#include "UI/ResultWidget.h"
#include "GameFramework/Pawn.h"
#include "System/DietGameState.h"
#include "Player/PlayerCharacter.h"
#include "Player/AttackComponent.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	ShowMainHUD();

	GetWorldTimerManager().SetTimerForNextTick(this, &AMainHUD::BindDelegates);

	if (APlayerController* PC = GetOwningPlayerController())
	{
		PC->OnPossessedPawnChanged.AddDynamic(this, &AMainHUD::HandlePossessedPawnChanged);
	}
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
	SetUIInputMode(false);
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

	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		SetUIInputMode(false);
	}
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

	BindPawnDelegates(PC->GetPawn());

	CachedGameState = GetWorld()->GetGameState<ADietGameState>();
	if (CachedGameState)
	{
		CachedGameState->OnWaveIncrease.AddDynamic(this, &AMainHUD::HandleWaveIncrease);
		CachedGameState->OnTimeUp.AddDynamic(this, &AMainHUD::HandleTimeUp);
		CachedGameState->UpdateElapsedTime.AddDynamic(this, &AMainHUD::HandleElapsedTimeUpdated);

		HandleWaveIncrease(CachedGameState->GetCurrentWave());
	}
}

void AMainHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CachedPlayerState)
	{
		CachedPlayerState->OnExpChanged.RemoveDynamic(this, &AMainHUD::HandleExpChanged);
		CachedPlayerState->OnLevelUp.RemoveDynamic(this, &AMainHUD::HandleLevelUp);
	}

	UnbindPawnDelegates();

	if (APlayerController* PC = GetOwningPlayerController())
	{
		PC->OnPossessedPawnChanged.RemoveDynamic(this, &AMainHUD::HandlePossessedPawnChanged);
	}

	if (CachedGameState)
	{
		CachedGameState->OnWaveIncrease.RemoveDynamic(this, &AMainHUD::HandleWaveIncrease);
		CachedGameState->OnTimeUp.RemoveDynamic(this, &AMainHUD::HandleTimeUp);
		CachedGameState->UpdateElapsedTime.RemoveDynamic(this, &AMainHUD::HandleElapsedTimeUpdated);
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
	UserHUDWidget->SetFullnessWarning(MaxFullness > 0.f ? NewFullness / MaxFullness : 0.f);
}

void AMainHUD::HandleWaveIncrease(int32 CurrentWave)
{
	if (!UserHUDWidget) { return; }
	UserHUDWidget->SetWave(CurrentWave);
}

void AMainHUD::HandleElapsedTimeUpdated(float ElapsedSeconds)
{
	if (!UserHUDWidget) { return; }
	UserHUDWidget->SetTimer(ElapsedSeconds);
}

void AMainHUD::HandleInvincibilityChanged(bool bIsNowInvincible)
{
	if (!UserHUDWidget || !bIsNowInvincible) { return; }
	UserHUDWidget->PlayHitFlash();
}

void AMainHUD::HandleAttackHit(AActor* HitActor, float DamageAmount)
{
	if (!UserHUDWidget) { return; }
	UserHUDWidget->PlayHitMarker();
}


void AMainHUD::BindPawnDelegates(APawn* Pawn)
{
	UnbindPawnDelegates();
	if (!Pawn || !UserHUDWidget) { return; }

	CachedStatComp = Pawn->FindComponentByClass<UPlayerStatComponent>();
	if (CachedStatComp)
	{
		CachedStatComp->OnFullnessChanged.AddDynamic(this, &AMainHUD::HandleFullnessChanged);
		CachedStatComp->OnFullnessMax.AddDynamic(this, &AMainHUD::HandleFullnessMax);

		HandleFullnessChanged(CachedStatComp->GetFullness(), CachedStatComp->GetMaxFullness());
	}

	CachedPlayerCharacter = Cast<APlayerCharacter>(Pawn);
	if (CachedPlayerCharacter)
	{
		CachedPlayerCharacter->OnInvincibilityChanged.AddDynamic(this, &AMainHUD::HandleInvincibilityChanged);
	}

	CachedAttackComp = Pawn->FindComponentByClass<UAttackComponent>();
	if (CachedAttackComp)
	{
		CachedAttackComp->OnAttackHit.AddDynamic(this, &AMainHUD::HandleAttackHit);
	}
}

void AMainHUD::UnbindPawnDelegates()
{
	if (CachedStatComp)
	{
		CachedStatComp->OnFullnessChanged.RemoveDynamic(this, &AMainHUD::HandleFullnessChanged);
		CachedStatComp->OnFullnessMax.RemoveDynamic(this, &AMainHUD::HandleFullnessMax);
		CachedStatComp = nullptr;
	}

	if (CachedPlayerCharacter)
	{
		CachedPlayerCharacter->OnInvincibilityChanged.RemoveDynamic(this, &AMainHUD::HandleInvincibilityChanged);
		CachedPlayerCharacter = nullptr;
	}

	if (CachedAttackComp)
	{
		CachedAttackComp->OnAttackHit.RemoveDynamic(this, &AMainHUD::HandleAttackHit);
		CachedAttackComp = nullptr;
	}
}

void AMainHUD::HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	BindPawnDelegates(NewPawn);
}

void AMainHUD::HandleFullnessMax()
{
	ShowResult(false);
}

void AMainHUD::HandleTimeUp()
{
	ShowResult(true);
}
