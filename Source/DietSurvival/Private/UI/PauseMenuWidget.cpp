#include "UI/PauseMenuWidget.h"
#include "UI/MainHUD.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::HandleResumeClicked);
	QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::HandleQuitClicked);
}

void UPauseMenuWidget::HandleResumeClicked()
{
	if (AMainHUD* HUD = Cast<AMainHUD>(GetOwningPlayer()->GetHUD()))
	{
		HUD->HidePauseMenu();
	}
}

void UPauseMenuWidget::HandleQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
