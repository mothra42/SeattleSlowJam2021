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
	CollisionComponent->OnComponentHit.AddDynamic(this, &AYarnBallProjectile::OnHit);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->AttachTo(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	//ProjectileMovement->UpdatedComponent = ProjectileMesh;
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

	ProjectileMovement->Velocity = GetActorRightVector() * 300.0f;

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

void AYarnBallProjectile::OnHit(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	AGhostEnemy* HitGhost = Cast<AGhostEnemy>(OtherActor);
	if (HitGhost && HitGhost->bAcceptYarnDamage)
	{
		HitGhost->Destroy();
	}
	else
	{
		//Add fire particle effect and destroy
	}

	Destroy();
}

void AYarnBallProjectile::LifeTimerOver()
{
	Destroy();
}

