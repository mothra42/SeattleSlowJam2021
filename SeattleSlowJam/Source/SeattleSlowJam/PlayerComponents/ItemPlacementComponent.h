// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemPlacementComponent.generated.h"

//TODO Need a method to adjust the line trace angle, but should be reset back after the item is placed.

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

	void FinishPlacingItem();

private:
	UPROPERTY(Category = "Item Placement Trace", EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float LineTraceLength = 150.0f;

	class APlaceableItem* CarriedItem = nullptr;

	APlaceableItem* GhostItem = nullptr;

	bool FindGhostItemPlacementLocation(FHitResult& Hit);

	void UpdateGhostItemLocation();

// getters and setters
public:
	FORCEINLINE void SetCarriedItem(APlaceableItem* ItemToCarry) { CarriedItem = ItemToCarry; }
	FORCEINLINE APlaceableItem* GetCarriedItem() const { return CarriedItem; }
};
