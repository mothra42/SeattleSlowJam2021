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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnGhostItem();

	void FinishPlacingItem();

	void RotateItem(bool bIsRightRotation);

	void AdjustLineTraceLength(bool bShouldIncrease);

	void MoveItemUp(float Value);

private:
	UPROPERTY(Category = "Item Placement Trace", EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DefaultLineTraceLength = 1000.0f;
	UPROPERTY(Category = "Item Placement Trace", EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MinDistanceToLineTrace = 600;
	UPROPERTY(Category = "Item Placement Trace", EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxDistanceToLineTrace = 1500;
	
	float LineTraceLength;

	float LineTraceAdjustmentAmount = 10.0f;

	class APlaceableItem* CarriedItem = nullptr;

	APlaceableItem* GhostItem = nullptr;

	bool FindGhostItemPlacementLocation(FHitResult& Hit);

	void UpdateGhostItemLocation();

	bool bAreOverlappedActorsValid();

// getters and setters
public:
	FORCEINLINE void SetCarriedItem(APlaceableItem* ItemToCarry) { CarriedItem = ItemToCarry; }
	FORCEINLINE APlaceableItem* GetCarriedItem() const { return CarriedItem; }
};
