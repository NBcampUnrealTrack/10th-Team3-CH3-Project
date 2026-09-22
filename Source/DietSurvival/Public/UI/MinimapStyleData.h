#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "MinimapStyleData.generated.h"

UENUM(BlueprintType)
enum class EMinimapIconType : uint8
{
	Enemy,
	Boss,
	Skill,
	Digestive,
	None,
};

USTRUCT(BlueprintType)
struct FMinimapIconStyle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FSlateBrush Brush;

	UPROPERTY(EditAnywhere)
	bool bClampToEdge = true;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ToolTip = "높을수록 위에 그려짐"))
	int32 Layer = 0;
};

UCLASS()
class DIETSURVIVAL_API UMinimapStyleData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Minimap")
	TMap<EMinimapIconType, FMinimapIconStyle> Styles;

	UPROPERTY(EditAnywhere, Category = "Minimap")
	FSlateBrush PlayerBrush;
};
