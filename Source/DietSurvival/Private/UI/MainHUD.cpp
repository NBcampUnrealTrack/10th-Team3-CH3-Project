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
#include "Player/AttackComponent.h"
#include "UI/DamageNumberWidget.h"
#include "UI/MinimapWidget.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	SetUIInputMode(false);

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

	if (MinimapWidgetClass)
	{
		MinimapWidget = CreateWidget<UMinimapWidget>(GetOwningPlayerController(), MinimapWidgetClass);
		if (MinimapWidget) { MinimapWidget->AddToViewport(static_cast<int32>(EUILayer::HUD)); }
	}
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
		CachedGameState->OnTimeUp.AddDynamic(this, &AMainHUD::HandleTimeUp);
		CachedGameState->UpdateElapsedTime.AddDynamic(this, &AMainHUD::HandleElapsedTimeUpdated);

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
	if (NewFullness > LastFullness) { UserHUDWidget->PlayHitFlash(); }
	LastFullness = NewFullness;
}

void AMainHUD::HandleElapsedTimeUpdated(float ElapsedSeconds)
{
	if (!UserHUDWidget) { return; }
	UserHUDWidget->SetTimer(ElapsedSeconds);
}

void AMainHUD::HandleAttackHit(AActor* HitActor, float DamageAmount)
{
	if (!UserHUDWidget) { return; }
	UserHUDWidget->PlayHitMarker();
	ShowDamageNumber(HitActor, DamageAmount);
}

void AMainHUD::HandleCurrentAmmoChanged(int32 CurrentAmmo)
{
	if (!UserHUDWidget) { return; }
	UserHUDWidget->SetAmmo(CurrentAmmo);
	UserHUDWidget->FinishReload();
}

void AMainHUD::HandleReloadStart()
{
	if (!UserHUDWidget) { return; }
	UserHUDWidget->StartReload();
}

void AMainHUD::ShowDamageNumber(AActor* HitActor, float Damage)
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC || !HitActor || !DamageNumberWidgetClass) { return; }

	UDamageNumberWidget* Widget = CreateWidget<UDamageNumberWidget>(PC, DamageNumberWidgetClass);
	if (!Widget) { return; }

	Widget->AttachToActor(HitActor);
	Widget->AddToViewport(static_cast<int32>(EUILayer::DamageNumber));
	Widget->SetDamage(Damage);
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
		LastFullness = CachedStatComp->GetFullness();
		HandleFullnessChanged(CachedStatComp->GetFullness(), CachedStatComp->GetMaxFullness());
	}

	CachedAttackComp = Pawn->FindComponentByClass<UAttackComponent>();
	if (CachedAttackComp)
	{
		CachedAttackComp->OnAttackHit.AddDynamic(this, &AMainHUD::HandleAttackHit);
		CachedAttackComp->OnCurrentAmmoChanged.AddDynamic(this, &AMainHUD::HandleCurrentAmmoChanged);
		CachedAttackComp->OnReloadStart.AddDynamic(this, &AMainHUD::HandleReloadStart);

		HandleCurrentAmmoChanged(CachedStatComp->GetMaxAmmo());
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

	if (CachedAttackComp)
	{
		CachedAttackComp->OnAttackHit.RemoveDynamic(this, &AMainHUD::HandleAttackHit);
		CachedAttackComp->OnCurrentAmmoChanged.RemoveDynamic(this, &AMainHUD::HandleCurrentAmmoChanged);
		CachedAttackComp->OnReloadStart.RemoveDynamic(this, &AMainHUD::HandleReloadStart);
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
