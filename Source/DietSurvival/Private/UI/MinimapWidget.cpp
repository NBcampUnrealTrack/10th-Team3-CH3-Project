#include "UI/MinimapWidget.h"
#include "UI/MinimapStyleData.h"
#include "UI/MinimapSubsystem.h"
#include "UI/MinimapTrackComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

static void DrawIcon(const FGeometry& Geometry, FSlateWindowElementList& OutDrawElements, int32 Layer, const FSlateBrush& Brush, const FVector2D& Center)
{
	const FVector2D Size = Brush.ImageSize;
	FSlateDrawElement::MakeBox(OutDrawElements, Layer,
		Geometry.ToPaintGeometry(Size, FSlateLayoutTransform(Center - Size * 0.5f)),
		&Brush, ESlateDrawEffect::None, Brush.TintColor.GetSpecifiedColor());
}

int32 UMinimapWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	LayerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	APlayerController* PC = GetOwningPlayer();
	APawn* Pawn = PC ? PC->GetPawn() : nullptr;
	UMinimapSubsystem* Subsystem = GetWorld() ? GetWorld()->GetSubsystem<UMinimapSubsystem>() : nullptr;
	if (!Pawn || !PC->PlayerCameraManager || !StyleData || !Subsystem) { return LayerId; }

	const FGeometry& MapGeometry = MapArea->GetPaintSpaceGeometry();
	const FVector2D Center = MapGeometry.GetLocalSize() * 0.5f;
	const float MapRadius = Center.X;
	const float Scale = MapRadius / WorldRange;

	const FVector PlayerLoc = Pawn->GetActorLocation();
	const FVector Forward = FRotator(0.f, PC->PlayerCameraManager->GetCameraRotation().Yaw, 0.f).Vector();
	const FVector Right(-Forward.Y, Forward.X, 0.f);

	int32 MaxLayer = 0;
	for (const UMinimapTrackComponent* Component : Subsystem->GetTracked())
	{
		if (!Component->IsTracked()) { continue; }

		const FMinimapIconStyle* Style = StyleData->Styles.Find(Component->GetIconType());
		if (!Style) { continue; }

		const FVector Offset = Component->GetOwner()->GetActorLocation() - PlayerLoc;
		FVector2D Pos(FVector::DotProduct(Offset, Right) * Scale, -FVector::DotProduct(Offset, Forward) * Scale);
		if (Pos.SizeSquared() > MapRadius * MapRadius)
		{
			if (!Style->bClampToEdge) { continue; }
			Pos = Pos.GetSafeNormal() * MapRadius;
		}

		DrawIcon(MapGeometry, OutDrawElements, LayerId + Style->Layer, Style->Brush, Center + Pos);
		MaxLayer = FMath::Max(MaxLayer, Style->Layer);
	}

	DrawIcon(MapGeometry, OutDrawElements, LayerId + MaxLayer + 1, StyleData->PlayerBrush, Center);
	return LayerId + MaxLayer + 1;
}
