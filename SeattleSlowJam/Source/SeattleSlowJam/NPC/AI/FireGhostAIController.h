// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FireGhostAIController.generated.h"

/**
 * 
 */
UCLASS()
class SEATTLESLOWJAM_API AFireGhostAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	void CheckIfPlayerInRadius();

	void PursuePlayer();

	FTimerHandle TimerHandle_CheckForPlayerTimerExpired;

	FTimerHandle TimerHandle_UpdatePlayerLocationTimerExpired;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float PlayerCheckTimer = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float UpdatePlayerLocationTimer = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float AcceptablePlayerRadius = 500.0f;

	void UpdateMoveTo();
protected:
	virtual void BeginPlay() override;

public:
	bool bShouldPursuePlayer = false;

	virtual void Tick(float DeltaTime) override;
};
