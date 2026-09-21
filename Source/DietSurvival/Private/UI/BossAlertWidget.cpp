#include "UI/BossAlertWidget.h"

void UBossAlertWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(ShowAnim);
}

void UBossAlertWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);

	if (Animation == ShowAnim)
	{
		RemoveFromParent();
	}
}
