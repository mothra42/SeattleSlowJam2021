// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemTeleportationArea.generated.h"

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

	int32 GetNumOfPlaceableItems();

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetShouldPortalDoorBeOpen() { return bShouldPortalDoorBeOpen; }
};
