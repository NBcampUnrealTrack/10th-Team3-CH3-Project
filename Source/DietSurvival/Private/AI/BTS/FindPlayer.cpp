// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTS/FindPlayer.h"
#include "BehaviorTree/BTFunctionLibrary.h"

void UFindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AActor* Player = nullptr;

	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, Player);
}
