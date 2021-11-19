// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemTeleportationArea.h"
#include "../CabinItems/PlaceableItem.h"
#include "Components/BoxComponent.h"

// Sets default values
AItemTeleportationArea::AItemTeleportationArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerArea"));
	TriggerArea->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AItemTeleportationArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemTeleportationArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 AItemTeleportationArea::GetNumOfPlaceableItems()
{

	TriggerArea->GetOverlappingActors(OverlappingActors);

	int32 count = 0;
	for (AActor* Actor : OverlappingActors)
	{
		if (Cast<APlaceableItem>(Actor) != nullptr)
		{
			count++;
		}
	}

	return count;
}

bool AItemTeleportationArea::CheckShouldPortalDoorOpen()
{
	if (GetNumOfPlaceableItems() <= 0)
	{
		bShouldPortalDoorBeOpen = true;
		UE_LOG(LogTemp, Warning, TEXT("Portal door is open"));
	}
	else
	{
		bShouldPortalDoorBeOpen = false;
		UE_LOG(LogTemp, Warning, TEXT("Portal door is closed"));
	}

	return bShouldPortalDoorBeOpen;
}

void AItemTeleportationArea::TeleportAllCollectedItems()
{
	for (FSpawnInstructions Instruction : SpawnInstructions)
	{
		GetWorld()->SpawnActor<APlaceableItem>(Instruction.ItemToSpawn, Instruction.Location, Instruction.Rotation);
	}
	ClearItemsToTeleport();
}

void AItemTeleportationArea::AddItemToTeleport(APlaceableItem* ItemToTeleport)
{
	FSpawnInstructions SpawnInstruction(
		ItemToTeleport->BasementTeleportLocation,
		ItemToTeleport->GetClass(),
		ItemToTeleport->GetActorRotation());
	SpawnInstructions.Add(SpawnInstruction);
	ItemToTeleport->Destroy();
}

void AItemTeleportationArea::ClearItemsToTeleport()
{
	SpawnInstructions.Empty();
}