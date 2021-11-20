// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "../CabinCharacter.h"

// Sets default values
AGhostEnemy::AGhostEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AGhostEnemy::OnBeginOverlap);
	RootComponent = CapsuleComponent;
	
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetupAttachment(CapsuleComponent);
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
}

// Called when the game starts or when spawned
void AGhostEnemy::BeginPlay()
{
	Super::BeginPlay();
	SetAmplitude();
	SetPeriod();
}

// Called every frame
void AGhostEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Cycle = CalculateSineCycle(DeltaTime);
	SetActorLocation(GetActorLocation() + 
		(FVector::ForwardVector * MovementSpeed) + 
		(FVector::UpVector * Amplitude * FMath::Sin(Cycle))
	);
}

float AGhostEnemy::CalculateSineCycle(float DeltaTime)
{
	return FMath::Fmod(((DeltaTime * 360.0) / Period) + Cycle, 360.0);
}

void AGhostEnemy::SetAmplitude()
{
	Amplitude = FMath::RandRange(MinAmplitude, MaxAmplitude);
}

void AGhostEnemy::SetPeriod()
{
	Period = FMath::RandRange(MinPeriod, MaxPeriod);
}

void AGhostEnemy::OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ACabinCharacter* CabinCharacter = Cast<ACabinCharacter>(OtherActor);
	if (CabinCharacter != nullptr)
	{
		ApplyDamage(CabinCharacter);
	}
}

void AGhostEnemy::ApplyDamage(ACabinCharacter* CabinCharacter)
{
	CabinCharacter->HandleDeath();
}