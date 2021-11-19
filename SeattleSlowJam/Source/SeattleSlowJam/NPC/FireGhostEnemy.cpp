// Fill out your copyright notice in the Description page of Project Settings.


#include "FireGhostEnemy.h"
#include "EnemyProjectiles/FireDrop.h"

void AFireGhostEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunSpawnTimer();
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

void AFireGhostEnemy::RunSpawnTimer()
{
	if (bCanSpawnFireDrop)
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_SpawnFireDropTimerExpired,
			this,
			&AFireGhostEnemy::SpawnTimerExpired,
			SpawnRate
		);

		bCanSpawnFireDrop = false;
		SpawnFireDrop();
	}
}

void AFireGhostEnemy::SpawnTimerExpired()
{
	bCanSpawnFireDrop = true;
}