#include "Item/ItemDropManager.h"
#include "Item/ExpItem.h"
#include "Item/DigestiveItem.h"
#include "Item/SkillAugmentItem.h"
#include "System/DietGameInstance.h"
#include "Pool/PoolManager.h"
#include "Pool/PoolObjectComponent.h"
#include "Kismet/GameplayStatics.h"

UItemDropManager* UItemDropManager::Get(const UObject* WorldContext)
{
	UDietGameInstance* DietGameInstance = UDietGameInstance::Get(WorldContext);
	if (DietGameInstance == nullptr)
	{
		return nullptr;
	}
	return DietGameInstance->GetSubsystem<UItemDropManager>();
}

void UItemDropManager::Initialize(FSubsystemCollectionBase& Collection)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[ItemDropManager::Initialize] World is nullptr"));
		return;
	}

	TArray<AActor*> FoundPoolManagers;
	UGameplayStatics::GetAllActorsOfClass(World, APoolManager::StaticClass(), FoundPoolManagers);

	if (FoundPoolManagers.Num() > 0)
	{
		CachedPoolManager = Cast<APoolManager>(FoundPoolManagers[0]);
	}
	else
	{
		CachedPoolManager = World->SpawnActor<APoolManager>();
	}

	if (CachedPoolManager == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[ItemDropManager::Initialize] PoolManager is nullptr"));
		return;
	}
	// 임시. Todo 프로젝트 세팅에서 접근할 수 있도록 변경하기
	static const FString ExpItemPath = TEXT("/Game/Blueprints/Test/BP_ExpItem.BP_ExpItem_C");
	static const FString DigestiveItemPath = TEXT("/Game/Blueprints/Test/BP_DigestiveItem.BP_DigestiveItem_C");
	static const FString SkillAugmentItemPath = TEXT("/Game/Blueprints/Test/BP_SkillAugmentItem.BP_SkillAugmentItem_C");

	ExpItemClass = StaticLoadClass(AExpItem::StaticClass(), nullptr, *ExpItemPath);
	if (ExpItemClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ItemDropManager] Failed to load ExpItemClass at path: %s"), *ExpItemPath);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[ItemDropManager] Successfully loaded ExpItemClass"));
	}
	DigestiveItemClass = StaticLoadClass(ADigestiveItem::StaticClass(), nullptr, *DigestiveItemPath);
	SkillAugmentItemClass = StaticLoadClass(ASkillAugmentItem::StaticClass(), nullptr, *SkillAugmentItemPath);
	///Script/Engine.Blueprint'/Game/Blueprints/Test/BP_ExpItem.BP_ExpItem'


	//if (ExpItemClass != nullptr)
	//{
	//	CachedPoolManager->AddPool(ExpItemClass, 10);
	//}
	//if (DigestiveItemClass != nullptr)
	//{
	//	CachedPoolManager->AddPool(DigestiveItemClass, 10);
	//}
	//if (SkillAugmentItemClass != nullptr)
	//{
	//	CachedPoolManager->AddPool(SkillAugmentItemClass, 10);
	//}
}

void UItemDropManager::Deinitialize()
{
	Super::Deinitialize();
}

void UItemDropManager::RequestDrop(FVector SpawnLocation)
{
	if (!bPoolsInitialized)
	{
		CachedPoolManager->AddPool(ExpItemClass, 10);
		CachedPoolManager->AddPool(DigestiveItemClass, 10);
		CachedPoolManager->AddPool(SkillAugmentItemClass, 10);
		bPoolsInitialized = true;
	}

	if (CachedPoolManager == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[ItemDropManager::RequestDrop] PoolManager is nullptr"));
		return;
	}
	// Exp는 무조건 드랍
	AActor* ExpItemActor = CachedPoolManager->GetPoolOjbect(ExpItemClass);
	if (ExpItemActor != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[ItemDropManager::RequestDrop] Drop Exp"));
		ExpItemActor->SetActorLocation(SpawnLocation);
		UPoolObjectComponent* Comp = ExpItemActor->FindComponentByClass<UPoolObjectComponent>();
		if (Comp != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("[ItemDropManager::RequestDrop] OnAcquire call"));
			Comp->OnAcquire();
		}
	}

	// 임시. Todo 프로젝트 세팅에서 확률값 수정할 수 있도록 변경하기
	const float DigestiveDropChance = 30.0f;
	if (FMath::FRandRange(0.0f, 100.0f) < DigestiveDropChance)
	{
		AActor* DigestiveItemActor = CachedPoolManager->GetPoolOjbect(DigestiveItemClass);
		if (DigestiveItemActor != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("[ItemDropManager::RequestDrop] Drop Digestive"));
			DigestiveItemActor->SetActorLocation(SpawnLocation);
		}
	}

	// 임시. Todo 프로젝트 세팅에서 확률값 수정할 수 있도록 변경하기
	const float SkillAugmentDropChance = 20.0f;
	if (FMath::FRandRange(0.0f, 100.0f) < SkillAugmentDropChance)
	{
		AActor* SkillItemActor = CachedPoolManager->GetPoolOjbect(SkillAugmentItemClass);
		if (SkillItemActor != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("[ItemDropManager::RequestDrop] Drop SkillAugment"));
			SkillItemActor->SetActorLocation(SpawnLocation);
		}
	}

}
