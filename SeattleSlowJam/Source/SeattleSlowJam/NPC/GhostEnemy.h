// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GhostEnemy.generated.h"

UCLASS()
class SEATTLESLOWJAM_API AGhostEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGhostEnemy();

protected:
	UPROPERTY(Category = "Ghost", EditDefaultsOnly, BlueprintReadOnly)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(Category = "Ghost", EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* EnemyMesh;

	UPROPERTY(Category = "Ghost", EditDefaultsOnly, BlueprintReadOnly)
	class UFloatingPawnMovement* MovementComponent;

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadOnly)
	float MinAmplitude;

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadOnly)
	float MaxAmplitude;

	float Amplitude;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
