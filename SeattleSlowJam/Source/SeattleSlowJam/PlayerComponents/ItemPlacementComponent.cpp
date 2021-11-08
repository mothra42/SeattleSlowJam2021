// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPlacementComponent.h"
#include "../CabinItems/PlaceableItem.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

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
	UpdateGhostItemLocation();
}

bool UItemPlacementComponent::FindGhostItemPlacementLocation(FHitResult& Hit)
{
	FVector StartLocation = GetOwner()->GetActorLocation();
	FRotator ActorRotation = GetOwner()->GetActorRotation();
	FVector TraceDirection = UKismetMathLibrary::CreateVectorFromYawPitch(ActorRotation.Yaw, DefaultPitchAdjustment);
	DrawDebugLine(GetWorld(), StartLocation, StartLocation + TraceDirection * LineTraceLength, FColor::Green, false, 5.0f);
	//TODO consider changing this to a trace by profile.
	return GetWorld()->LineTraceSingleByChannel(Hit,
		StartLocation,
		StartLocation + TraceDirection * LineTraceLength,
		ECollisionChannel::ECC_Camera);
}

//This method should only be called when the player first picks up and item;
void UItemPlacementComponent::SpawnGhostItem()
{
	FHitResult Hit;

	if (FindGhostItemPlacementLocation(Hit) && CarriedItem != nullptr)
	{
		GhostItem = GetWorld()->SpawnActor<APlaceableItem>(Hit.Location, FRotator());
		GhostItem->SetItemStaticMesh(CarriedItem->GetStaticMesh());
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
}

