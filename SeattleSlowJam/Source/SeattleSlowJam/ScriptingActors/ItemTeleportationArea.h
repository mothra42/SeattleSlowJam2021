// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemTeleportationArea.generated.h"

USTRUCT()
struct FSpawnInstructions
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	TSubclassOf<class APlaceableItem> ItemToSpawn;
	FRotator Rotation;

	FSpawnInstructions()
		: Location(FVector()), ItemToSpawn(nullptr), Rotation(FRotator()) {}

	FSpawnInstructions(const FVector InLocation, const TSubclassOf<APlaceableItem>& InItemClassToSpawn, const FRotator InRotator)
		: Location(InLocation), ItemToSpawn(InItemClassToSpawn), Rotation(InRotator) {}
};

UCLASS()
class SEATTLESLOWJAM_API AItemTeleportationArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemTeleportationArea();

	UPROPERTY(Category = "AreaSize", EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* TriggerArea;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool CheckShouldPortalDoorOpen();

	UFUNCTION(BlueprintCallable)
	void TeleportAllCollectedItems();

	void AddItemToTeleport(class APlaceableItem* ItemToTeleport);

	void ClearItemsToTeleport();

private:
	bool bShouldPortalDoorBeOpen = true;

	TArray<AActor*> OverlappingActors;

	TArray<APlaceableItem*> ItemsToTeleport;

	TArray<FSpawnInstructions> SpawnInstructions;

	int32 GetNumOfPlaceableItems();

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetShouldPortalDoorBeOpen() { return bShouldPortalDoorBeOpen; }

	UFUNCTION(BlueprintCallable)
	void SetPortalDoorClosed();
};
