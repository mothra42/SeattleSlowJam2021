// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "YarnBallProjectile.generated.h"

UCLASS()
class SEATTLESLOWJAM_API AYarnBallProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AYarnBallProjectile();

protected:
	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(Category = "Setup", EditDefaultsOnly, BlueprintReadOnly)
	class USphereComponent* CollisionComponent;

	UPROPERTY(Category = "Setup", EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* StaticMeshComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//lifetime timer
	FTimerHandle TimerHandle_LifeTime;

	UPROPERTY(Category = "Setup", EditDefaultsOnly, BlueprintReadOnly)
	float Lifetime = 5.0f;

	void LifeTimerOver();

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
