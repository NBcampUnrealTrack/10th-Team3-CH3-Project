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
#include "System/EnemyDataRow.h"
#include "System/DietPlayerState.h"
#include "Item/ItemDropManager.h"

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

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	PowerAttack = 0;
	Exp = 0;

	Tags.Add(FName("Enemy"));
}

void ADietEnemyBase::InitAttritube(const FEnemyDataRow& EnemyDataRow)
{
	if (EnemyDataRow.Health == 0) return;
	PowerAttack = EnemyDataRow.PowerAttack;
	Exp = EnemyDataRow.Exp;
	HealthComponent->Initailize(EnemyDataRow.Health);
}

float ADietEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount <= 0) return DamageAmount;
	if (bIsDead) return DamageAmount;
	ControllerLastAttacked = EventInstigator;
	HealthComponent->TakeDamage(DamageAmount);
	return DamageAmount;
}

void ADietEnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) {
		UE_LOG(LogTemp, Warning,
			TEXT("ADietEnemyBase::PossessedBy, Not Authority"));
		return;
	}

	DietAIController = Cast<ADietAIController>(NewController);

	if (!DietAIController) {
		UE_LOG(LogTemp, Warning,
			TEXT("ADietEnemyBase::PossessedBy, DietAIController is Null"));
		return;
	}

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
		UE_LOG(LogTemp, Warning,
			TEXT("ADietEnemyBase::RunAI, DietAIController is Null, Init Cast"));
		DietAIController =
			Cast<ADietAIController>(GetController());
	}

	if (!DietAIController)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("ADietEnemyBase::RunAI, DietAIController is Null"));
		return;
	}

	bIsDead = false;
	PoolObjectComponent->OnAcquire();

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

void ADietEnemyBase::HandleDeath()
{
	if (bIsDead) return;
	bIsDead = true;
	StopAI();

	UItemDropManager* DropManager = UItemDropManager::Get(this);

	if (ControllerLastAttacked) {
		ADietPlayerState* DietPlayerState = ControllerLastAttacked->GetPlayerState<ADietPlayerState>();
		if (DietPlayerState && DropManager) {
			// Todo Exp 아이템으로 이관하기
			DropManager->RequestDrop(GetActorLocation());
		}
		else {
			UE_LOG(LogTemp, Warning,
				TEXT("ADietEnemyBase::HandleDeath, DietPlayerState is Null"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning,
			TEXT("ADietEnemyBase::HandleDeath, ControllerLastAttacked is Null"));
	}

	if (PoolObjectComponent) {
		PoolObjectComponent->ReturnToPool();
	}
	else {
		UE_LOG(LogTemp, Warning,
			TEXT("ADietEnemyBase::HandleDeath, PoolObjectComponent is Null"));
	}
}

void ADietEnemyBase::OnCapsuleOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	if (!OtherActor
		|| !OtherActor->ActorHasTag("Player")) return;

	AttackToTarget(OtherActor);
}

void ADietEnemyBase::AttackToTarget(AActor* Target)
{
	UGameplayStatics::ApplyDamage(
		Target,
		PowerAttack,
		GetController(),
		this,
		UDamageType::StaticClass()
	);

	StopAI();
	if (PoolObjectComponent) {
		PoolObjectComponent->ReturnToPool();
	}
	else {
		UE_LOG(LogTemp, Warning,
			TEXT("ADietEnemyBase::AttackToTarget, PoolObjectComponent is Null"));
	}
}

