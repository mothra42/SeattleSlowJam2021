// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPlacementComponent.h"
#include "../CabinItems/PlaceableItem.h"
#include "Camera/CameraComponent.h"
#include "../CabinCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "../ScriptingActors/ItemTeleportationArea.h"

// Sets default values for this component's properties
UItemPlacementComponent::UItemPlacementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UItemPlacementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UItemPlacementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ACabinCharacter* Owner = Cast<ACabinCharacter>(GetOwner());
	if (CarriedItem != nullptr && Owner != nullptr && !Owner->GetItemModeState())
	{
		UpdateGhostItemLocation();
	}
}

bool UItemPlacementComponent::FindGhostItemPlacementLocation(FHitResult& Hit)
{
	ACabinCharacter* PlayerCharacter = Cast<ACabinCharacter>(GetOwner());
	FVector StartLocation = PlayerCharacter->GetFollowCamera()->GetComponentLocation();
	FVector EndLocation = StartLocation + PlayerCharacter->GetFollowCamera()->GetForwardVector() * LineTraceLength;
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	//MAYBE consider changing this to a trace by profile
	return GetWorld()->LineTraceSingleByObjectType(Hit,
		StartLocation,
		EndLocation,
		ObjectParams);
}

//This method should only be called when the player first picks up and item;
void UItemPlacementComponent::SpawnGhostItem()
{
	FHitResult Hit;
	LineTraceLength = FMath::Clamp(DefaultLineTraceLength, 650.0f, 1500.0f); //not working, maybe clamp doesn't leave scope.
	if (FindGhostItemPlacementLocation(Hit) && CarriedItem != nullptr)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FRotator CarriedItemRotation = CarriedItem->GetActorRotation();
		GhostItem = GetWorld()->SpawnActor<APlaceableItem>(Hit.ImpactPoint, CarriedItemRotation, ActorSpawnParams);
		GhostItem->SetActorScale3D(CarriedItem->GetActorScale3D());
		GhostItem->GetStaticMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		GhostItem->SetItemStaticMesh(CarriedItem->GetStaticMesh());
		GhostItem->GetStaticMesh()->SetRelativeScale3D(CarriedItem->GetStaticMesh()->GetRelativeScale3D());
		GhostItem->GetStaticMesh()->SetMaterial(0, CarriedItem->GetStaticMesh()->GetMaterial(0));
		GhostItem->bCanBePlacedOnWall = CarriedItem->bCanBePlacedOnWall;
		//TODO give the ghost item a transparent material;
	}
}

void UItemPlacementComponent::UpdateGhostItemLocation()
{
	FHitResult Hit;
	if (FindGhostItemPlacementLocation(Hit) && GhostItem != nullptr)
	{
		GhostItem->SetActorLocation(Hit.Location);
	}
	else
	{
		//TODO Apply a color that makes it clear it's an invalid placement.
		ACabinCharacter* PlayerCharacter = Cast<ACabinCharacter>(GetOwner());
		FVector StartLocation = PlayerCharacter->GetFollowCamera()->GetComponentLocation();
		FVector EndLocation = StartLocation + PlayerCharacter->GetFollowCamera()->GetForwardVector() * LineTraceLength;
		GhostItem->SetActorLocation(EndLocation);
	}

	if (!bAreOverlappedActorsValid())
	{
		//make object red showing its unplaceable
		UE_LOG(LogTemp, Warning, TEXT("Overlapping actors"));
	}
}

void UItemPlacementComponent::FinishPlacingItem()
{
	TArray<AActor*> OverlappedActors;
	TArray<AActor*> OverlappedPlaceableItems;
	GhostItem->GetOverlappingActors(OverlappedActors);
	TSubclassOf<APlaceableItem> PlaceableItemClass;
	GhostItem->GetOverlappingActors(OverlappedPlaceableItems, PlaceableItemClass);
	if (bAreOverlappedActorsValid())
	{
		FVector NewLocation = GhostItem->GetActorLocation();
		FRotator NewRotation = GhostItem->GetActorRotation();
		GhostItem->Destroy();
		GhostItem = nullptr;
		CarriedItem->SetActorLocationAndRotation(NewLocation, NewRotation);
		CarriedItem->SetActorHiddenInGame(false);
		CarriedItem->GetStaticMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		AItemTeleportationArea* TeleportationArea = CarriedItem->GetTeleportationArea();
		if (TeleportationArea != nullptr)
		{
			TeleportationArea->CheckShouldPortalDoorOpen();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlaceableItem %s, has no set teleportation area!"), *CarriedItem->GetName());
		}
		CarriedItem = nullptr;
	}
}

void UItemPlacementComponent::RotateItem(bool bIsRightRotation)
{
	if (CarriedItem != nullptr && GhostItem != nullptr)
	{
		GhostItem->RotateRight(bIsRightRotation);
	}
}

void UItemPlacementComponent::AdjustLineTraceLength(bool bShouldIncrease)
{
	//TODO clamp and have default value between carries;
	if(bShouldIncrease)
	{
		LineTraceLength += LineTraceAdjustmentAmount;
	}
	else
	{
		LineTraceLength -= LineTraceAdjustmentAmount;
	}
}

void UItemPlacementComponent::MoveItemUp(float Value)
{
	if (Value > 0.0f)
	{
		GhostItem->AdjustHeight(true);
	}
	else
	{
		GhostItem->AdjustHeight(false);
	}
}

bool UItemPlacementComponent::bAreOverlappedActorsValid()
{
	TArray<AActor*> OverlappedActors;
	TArray<AActor*> OverlappedPlaceableItems;
	GhostItem->GetOverlappingActors(OverlappedActors);
	TSubclassOf<APlaceableItem> PlaceableItemClass;
	GhostItem->GetOverlappingActors(OverlappedPlaceableItems, PlaceableItemClass);

	return (OverlappedActors.Num() == OverlappedPlaceableItems.Num() || OverlappedActors.Num() <= 0);
}