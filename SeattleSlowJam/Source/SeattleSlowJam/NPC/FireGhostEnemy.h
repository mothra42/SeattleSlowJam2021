// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GhostEnemy.h"
#include "FireGhostEnemy.generated.h"

/**
 * 
 */
UCLASS()
class SEATTLESLOWJAM_API AFireGhostEnemy : public AGhostEnemy
{
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "Projectile", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AFireDrop> FireDropSpawnClass;

	void SpawnFireDrop();
private:
	//Spawn Timer
	FTimerHandle TimerHandle_SpawnFireDropTimerExpired;

	bool bCanSpawnFireDrop = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	float SpawnRate = 0.7f;

	void RunSpawnTimer();

	void SpawnTimerExpired();

public:
	virtual void Tick(float DeltaTime) override;
	
};
