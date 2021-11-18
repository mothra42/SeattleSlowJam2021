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
	TSubclassOf<class AFireDrop> FireDropSpawnClass;

	void SpawnFireDrop();

public:
	virtual void Tick(float DeltaTime) override;
	
};
