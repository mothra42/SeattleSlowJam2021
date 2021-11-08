// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemPlacementComponent.generated.h"


//TODO tomorrow add another method that will fire on tick, maybe optimizie this better down the line
//this function will be similar to spawn ghost item, it just won't spawn the the item.
//Need one more additional function to place the actual item from the player in the level.
//Need a method to adjust the line trace angle, but should be reset back after the item is placed.

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SEATTLESLOWJAM_API UItemPlacementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemPlacementComponent();

	UPROPERTY(Category = "Item Placement Trace", EditDefaultsOnly, BlueprintReadOnly)
	float DefaultPitchAdjustment = -45.0f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnGhostItem();

private:
	UPROPERTY(Category = "Item Placement Trace", EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float LineTraceLength = 150.0f;

	class APlaceableItem* CarriedItem;

	APlaceableItem* GhostItem;

	bool FindGhostItemPlacementLocation(FHitResult& Hit);

	void UpdateGhostItemLocation();

// getters and setters
public:
	FORCEINLINE void SetCarriedItem(APlaceableItem* ItemToCarry) { CarriedItem = ItemToCarry; }
};
