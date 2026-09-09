// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTS/FindPlayer.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "System/DietGameState.h"

UFindPlayer::UFindPlayer()
{
	NodeName = "FindPlayer Service";
}

void UFindPlayer::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	UWorld* World = GetWorld();

	if (!World) return;

	DietGameState = Cast<ADietGameState>(World->GetGameState());
}

void UFindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!DietGameState) {
		UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, nullptr);
		return;
	}
	if (!DietGameState->GetPlayerRef().IsValid()) {
		UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, nullptr);
		return;
	}
	AActor* Player = DietGameState->GetPlayerRef().Get();

	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, Player);
}
