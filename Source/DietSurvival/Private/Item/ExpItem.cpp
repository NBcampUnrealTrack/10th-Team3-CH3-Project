#include "item/ExpItem.h"
#include "System/DietPlayerState.h"

AExpItem::AExpItem()
{
	// Todo: EnemyDataRow에서 가져오는 걸로 변경하기
	ExpAmount = 10;
}

void AExpItem::ActivateItem(APawn* Activator)
{
	Super::ActivateItem(Activator);
	UE_LOG(LogTemp, Log, TEXT("[ExpItem] Activated by %s, Exp: %d"), *Activator->GetName(), ExpAmount);
	ADietPlayerState* DietPlayerState = Activator->GetPlayerState<ADietPlayerState>();
	if (DietPlayerState == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[DigestiveItem] DietPlayerState not found"));
		return;
	}

	DietPlayerState->GainExp(ExpAmount);

}

FName AExpItem::GetItemType()
{
	return FName("ExpItem");
}
