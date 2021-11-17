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
	float MinAmplitude = 2.0f;

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadOnly)
	float MaxAmplitude = 3.5f;

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadOnly)
	float MinPeriod = 200.0f;

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadOnly)
	float MaxPeriod = 300.0f;

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadOnly)
	float MovementSpeed = 2.0f;

	float Cycle = 0.0f;

	float Amplitude;

	float Period;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float CalculateSineCycle(float DeltaTime);

	void SetAmplitude();

	void SetPeriod();

	void ApplyDamage(class ACabinCharacter* CabinCharacter);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

};
