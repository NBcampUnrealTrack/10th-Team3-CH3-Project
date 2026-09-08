#include "UI/DamageNumberWidget.h"
#include "Components/TextBlock.h"

void UDamageNumberWidget::SetDamage(float Damage)
{
	DamageText->SetText(FText::AsNumber(FMath::RoundToInt(Damage)));
	PlayAnimation(FloatUpAnim);
}
