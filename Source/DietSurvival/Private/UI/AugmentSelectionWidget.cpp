#include "UI/AugmentSelectionWidget.h"
#include "Components/Button.h"

void UAugmentSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RerollButton->OnClicked.AddDynamic(this, &UAugmentSelectionWidget::HandleRerollClicked);
}

void UAugmentSelectionWidget::HandleRerollClicked()
{
	// 리롤 여러 번 누르는 상황 방지.
	RerollButton->SetIsEnabled(false);
	OnRerollPressed.Broadcast();
}
