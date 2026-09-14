#include "UI/DamageNumberWidget.h"
#include "Components/TextBlock.h"

void UDamageNumberWidget::SetDamage(float Damage)
{
	DamageText->SetText(FText::AsNumber(FMath::RoundToInt(Damage)));
	PlayAnimation(FloatUpAnim);
}

void UDamageNumberWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);

	if (Animation == FloatUpAnim)
	{
		RemoveFromParent();
	}
}

void UDamageNumberWidget::AttachToActor(AActor* Target)
{
	TrackedActor = Target;
	SetAlignmentInViewport(FVector2D(0.5f, 1.f));
}

void UDamageNumberWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	AActor* Target = TrackedActor.Get();
	APlayerController* PC = GetOwningPlayer();
	if (!Target || !PC) { return; }

	FVector2D ScreenPos;
	if (PC->ProjectWorldLocationToScreen(Target->GetActorLocation() + FVector(0.f, 0.f, 100.f), ScreenPos))
	{
		SetPositionInViewport(ScreenPos);
	}
}
