// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Skill/AuraOrbActor.h"

// Sets default values
AAuraOrbActor::AAuraOrbActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAuraOrbActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAuraOrbActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

