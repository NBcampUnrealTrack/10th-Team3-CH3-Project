#include "Item/DigestiveItem.h"
#include "Player/PlayerStatComponent.h"

ADigestiveItem::ADigestiveItem()
{
	DigestiveAmount = 10.f;
}

void ADigestiveItem::ActivateItem(APawn* Activator)
{
	Super::ActivateItem(Activator);
	UE_LOG(LogTemp, Log, TEXT("[DigestiveItem] Activated by %s"), *Activator->GetName());
	UPlayerStatComponent* StatComp = Activator->FindComponentByClass<UPlayerStatComponent>();
	if (StatComp == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[DigestiveItem] StatComponent not found"));
		return;
	}
	StatComp->AddFullness(-DigestiveAmount);
}

FName ADigestiveItem::GetItemType()
{
	return FName("DigestiveItem");
}
