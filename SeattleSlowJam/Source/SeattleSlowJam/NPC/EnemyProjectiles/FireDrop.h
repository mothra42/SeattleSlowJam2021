// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireDrop.generated.h"

UCLASS()
class SEATTLESLOWJAM_API AFireDrop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireDrop();

protected:
	UPROPERTY(Category = Setup, EditDefaultsOnly, BlueprintReadOnly)
	class USphereComponent* CollisionComponent;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
