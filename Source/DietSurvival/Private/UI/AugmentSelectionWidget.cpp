#include "UI/AugmentSelectionWidget.h"
#include "Components/Button.h"

void UAugmentSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RerollButton->OnClicked.AddDynamic(this, &UAugmentSelectionWidget::HandleRerollClicked);
}

void UAugmentSelectionWidget::HandleRerollClicked()
{
	// 리롤은 한 번만
	bRerollUsed = true;
	RerollButton->SetIsEnabled(false);
	OnRerollPressed.Broadcast();
}

void UAugmentSelectionWidget::SetInteractionEnabled(bool bEnabled)
{
	Super::SetInteractionEnabled(bEnabled);
	RerollButton->SetIsEnabled(bEnabled && !bRerollUsed);
}
