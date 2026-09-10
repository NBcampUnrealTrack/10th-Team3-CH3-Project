// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTS/FindPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "System/DietGameState.h"

UFindPlayer::UFindPlayer()
{
	NodeName = "FindPlayer Service";
}

void UFindPlayer::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	UWorld* World = GetWorld();

	if (!World) {
		UE_LOG(LogTemp, Warning,
			TEXT("UFindPlayer::OnSearchStart, World is Null"));
		return;
	}

	DietGameState = Cast<ADietGameState>(World->GetGameState());
}

void UFindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) {
		UE_LOG(LogTemp, Warning,
			TEXT("UFindPlayer::TickNode, BlackboardComp is Null"));
		return;
	}

	if (!DietGameState) {
		UE_LOG(LogTemp, Warning,
			TEXT("UFindPlayer::TickNode, DietGameState is Null"));

		BlackboardComp->SetValueAsObject(
			TargetToFollowSelector.SelectedKeyName, nullptr
		);
		return;
	}
	if (!DietGameState->GetPlayerRef().IsValid()) {
		UE_LOG(LogTemp, Warning,
			TEXT("UFindPlayer::TickNode, GetPlayerRef is Null"));

		BlackboardComp->SetValueAsObject(
			TargetToFollowSelector.SelectedKeyName, nullptr
		);
		return;
	}
	AActor* PlayerRef = DietGameState->GetPlayerRef().Get();

	UE_LOG(LogTemp, Warning,
		TEXT("UFindPlayer::TickNode, Player name : %s"), *GetNameSafe(PlayerRef));

	BlackboardComp->SetValueAsObject(
		TargetToFollowSelector.SelectedKeyName, PlayerRef
	);
	UE_LOG(LogTemp, Warning,
		TEXT("UFindPlayer::TickNode, Player is Set"));
}
