// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaceableItem.generated.h"

UCLASS()
class SEATTLESLOWJAM_API APlaceableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaceableItem();

protected:
	//Variables
	class USceneComponent* Root;

	UPROPERTY(Category = "ItemSettings", EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(Category = "ItemSettings", EditDefaultsOnly)
	float RotationAmountDegrees = 15.0f;

	UPROPERTY(Category = "ItemSettings", EditAnywhere)
	float ZMovementStepSize = 10.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(Category = "ItemSettings", EditAnywhere)
	bool bCanBePlacedOnWall = false;

	UPROPERTY(Category = "TeleportSettings", EditAnywhere, BlueprintReadWrite)
	FVector BasementTeleportLocation;

	bool bIsCarried = false;

	bool bIsPreparedToTeleport = false;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RotateRight(bool bIsRightRotation);

	void AdjustHeight(bool bIsUp);

	void PrepareToTeleportToBasement(class ATeleportationArea* TeleportationArea);

	void TeleportToBasement();

	void ChangeCollisionResponse(ECollisionResponse ResponseToSet);

	FORCEINLINE UStaticMeshComponent* GetStaticMesh() const { return ItemMesh; }
	FORCEINLINE void SetItemStaticMesh(UStaticMeshComponent* ItemMeshToSet) { ItemMesh->SetStaticMesh(ItemMeshToSet->GetStaticMesh()); }
//	FORCEINLINE AItemTeleportationArea* GetTeleportationArea() const { return TeleportationArea; }

};
