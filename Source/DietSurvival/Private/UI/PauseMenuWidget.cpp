#include "UI/PauseMenuWidget.h"
#include "UI/MainHUD.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::HandleResumeClicked);
	QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::HandleQuitClicked);
	MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::HandleMainMenuClicked);
}

void UPauseMenuWidget::HandleResumeClicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
	{
		HUD->HidePauseMenu();
	}
}

void UPauseMenuWidget::HandleQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}


void UPauseMenuWidget::HandleMainMenuClicked()
{
	UGameplayStatics::OpenLevel(this, MainMenuLevelName);
}
