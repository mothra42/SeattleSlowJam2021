// Fill out your copyright notice in the Description page of Project Settings.


#include "FireGhostEnemy.h"
#include "EnemyProjectiles/FireDrop.h"

void AFireGhostEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_SpawnFireDropTimer,
		this,
		&AFireGhostEnemy::SpawnFireDrop,
		SpawnRate,
		true
	);
}

void AFireGhostEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFireGhostEnemy::SpawnFireDrop()
{
	if (FireDropSpawnClass != nullptr)
	{
		const FVector SpawnLocation = GetActorLocation();
		const FRotator SpawnRotation = GetActorRotation();
		GetWorld()->SpawnActor<AFireDrop>(FireDropSpawnClass, SpawnLocation, SpawnRotation);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has no FireDropSpawnClass!"), *GetName());
	}
}