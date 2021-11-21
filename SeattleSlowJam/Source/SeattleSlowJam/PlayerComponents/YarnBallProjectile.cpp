// Fill out your copyright notice in the Description page of Project Settings.


#include "YarnBallProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../NPC/GhostEnemy.h"

// Sets default values
AYarnBallProjectile::AYarnBallProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AYarnBallProjectile::OnBeginOverlap);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->AttachTo(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::X);
	ProjectileMovement->bConstrainToPlane = true;
	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity
}

// Called when the game starts or when spawned
void AYarnBallProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovement->Velocity = GetActorRightVector() * 700.0f;

	//Destroy actor at the end of life time
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_LifeTime,
		this,
		&AYarnBallProjectile::LifeTimerOver,
		Lifetime);
}

// Called every frame
void AYarnBallProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AYarnBallProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit Actor is %s"), *OtherActor->GetName());
	AGhostEnemy* HitGhost = Cast<AGhostEnemy>(OtherActor);
	if (HitGhost && HitGhost->bAcceptYarnDamage)
	{
		HitGhost->Destroy();
	}
	else
	{
		PlayFireParticleEffect();
	}

	Destroy();
}

void AYarnBallProjectile::LifeTimerOver()
{
	Destroy();
}

