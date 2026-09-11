#include "UI/MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleStartClicked);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleExitClicked);

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(TakeWidget());
	PC->SetInputMode(InputMode);
	PC->SetShowMouseCursor(true);
}

void UMainMenuWidget::HandleStartClicked()
{
	UGameplayStatics::OpenLevel(this, GameLevelName);
}

void UMainMenuWidget::HandleExitClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
