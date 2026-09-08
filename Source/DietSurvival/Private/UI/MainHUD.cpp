#include "UI/MainHUD.h"
#include "UI/UserHUDWidget.h"
#include "UI/PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	ShowMainHUD();
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

	// [내일] 델리게이트 연결되면 삭제
	UserHUDWidget->SetHealth(70.f, 100.f);
	UserHUDWidget->SetExp(3, 10);
	UserHUDWidget->SetLevel(10);
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
