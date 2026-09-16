#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapWidget.generated.h"

class UMinimapStyleData;

UCLASS()
class DIETSURVIVAL_API UMinimapWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	UPROPERTY(EditDefaultsOnly, Category = "Minimap")
	TObjectPtr<UMinimapStyleData> StyleData;

	UPROPERTY(EditDefaultsOnly, Category = "Minimap", meta = (ClampMin = "1", Units = "cm"))
	float WorldRange = 3000.f;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidget> MapArea;
};
