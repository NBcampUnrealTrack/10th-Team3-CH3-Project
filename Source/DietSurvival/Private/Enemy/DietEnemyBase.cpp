// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DietEnemyBase.h"
#include "Enemy/DietAIController.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/Component/HealthComponent.h"
#include "Pool/PoolObjectComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADietEnemyBase::ADietEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;
	UCapsuleComponent* Collision = GetCapsuleComponent();
	if (Collision) {
		Collision->OnComponentBeginOverlap.AddDynamic(this, &ADietEnemyBase::OnCapsuleOverlap);
	}

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(GetRootComponent());

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
	HealthComponent->OnDeath.AddDynamic(this, &ADietEnemyBase::HandleDeath);

	PoolObjectComponent = CreateDefaultSubobject<UPoolObjectComponent>("PoolObject");

	PowerAttack = 0;
}
}

void ADietEnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;

	DietAIController = Cast<ADietAIController>(NewController);

	if (!DietAIController) return;

	UBlackboardComponent* Blackboard = DietAIController->GetBlackboardComponent();

	if (Blackboard && BehaviorTree)
	{
		Blackboard->InitializeBlackboard(
			*BehaviorTree->BlackboardAsset
		);
	}
}

void ADietEnemyBase::RunAI()
{
	if (!DietAIController)
	{
		DietAIController =
			Cast<ADietAIController>(GetController());
	}

	if (!DietAIController)
	{
		return;
	}

	// BT 재시작
	DietAIController->RunBehaviorTree(BehaviorTree);
}

void ADietEnemyBase::StopAI()
{
	if (!DietAIController) return;

	FString ObjectName = this->GetName();
	DietAIController->BrainComponent->StopLogic(FString::Printf(TEXT("Return To Pool, Name : %s"), *ObjectName));
	DietAIController->StopMovement();

	GetCharacterMovement()->StopMovementImmediately();

	GetWorldTimerManager().ClearAllTimersForObject(this);

}

// Called when the game starts or when spawned
void ADietEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

