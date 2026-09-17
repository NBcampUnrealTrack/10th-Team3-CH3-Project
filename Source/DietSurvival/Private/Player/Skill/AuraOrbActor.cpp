#include "Player/Skill/AuraOrbActor.h"
#include "Player/PlayerCharacter.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AAuraOrbActor::AAuraOrbActor()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// 물리적으로 밀어내지 않고, 겹침(Overlap)만 감지하도록 설정.
	// Pawn 채널에 대해서만 Overlap으로 반응하고, 나머지는 다 무시
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MeshComponent->SetGenerateOverlapEvents(true);

	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuraOrbActor::OnOrbOverlap);

}

void AAuraOrbActor::InitializeOrb(APlayerCharacter* InOwner, float InOrbitRadius, float InOrbitSpeed, float InBaseAngleOffset)
{
	OwnerCharacter = InOwner;
	OrbitRadius = InOrbitRadius;
	OrbitSpeed = InOrbitSpeed;
	BaseAngleOffset = InBaseAngleOffset;
}

void AAuraOrbActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerCharacter* InOwner = OwnerCharacter.Get();
	if (!InOwner)
	{
		return;
	}

	// 각 오브가 "자기 스폰 이후 흐른 시간"이 아니라 "월드 공용 시간"을 기준으로 각도를 계산해야
	// 나중에 새 오브가 추가돼도 위상(phase)이 어긋나지 않고 정확히 균등한 간격을 유지함
	const UWorld* World = GetWorld();
	const float WorldTime = World ? World->GetTimeSeconds() : 0.f;

	const float CurrentAngleDegrees = BaseAngleOffset + OrbitSpeed * WorldTime;
	const float AngleRadians = FMath::DegreesToRadians(CurrentAngleDegrees);

	const FVector Offset(FMath::Cos(AngleRadians) * OrbitRadius, FMath::Sin(AngleRadians) * OrbitRadius, 0.f);
	SetActorLocation(InOwner->GetActorLocation() + Offset);
}

void AAuraOrbActor::OnOrbOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* InOwner = OwnerCharacter.Get();
	if (!InOwner || !OtherActor || !OtherActor->ActorHasTag(TEXT("Enemy")))
	{
		return;
	}

	// 증강에서 정해준 데미지로
	UGameplayStatics::ApplyDamage(OtherActor, Damage, InOwner->GetInstigatorController(), InOwner, UDamageType::StaticClass());

	UE_LOG(LogTemp, Log, TEXT("[AuraOrbActor] %s에게 %.1f 데미지 적용 (오라 접촉)"), *OtherActor->GetName(), Damage);
}
