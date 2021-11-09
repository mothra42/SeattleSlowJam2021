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
	if (CarriedItem != nullptr)
	{
		UpdateGhostItemLocation();
	}
}

bool UItemPlacementComponent::FindGhostItemPlacementLocation(FHitResult& Hit)
{
	FVector StartLocation = GetOwner()->GetActorLocation();
	FRotator ActorRotation = GetOwner()->GetActorRotation();
	FVector TraceDirection = UKismetMathLibrary::CreateVectorFromYawPitch(ActorRotation.Yaw, DefaultPitchAdjustment);
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	DrawDebugLine(GetWorld(), StartLocation, StartLocation + TraceDirection * LineTraceLength, FColor::Green, false, 5.0f);
	//TODO consider changing this to a trace by profile
	return GetWorld()->LineTraceSingleByObjectType(Hit,
		StartLocation,
		StartLocation + TraceDirection * LineTraceLength,
		ObjectParams);
}

//This method should only be called when the player first picks up and item;
void UItemPlacementComponent::SpawnGhostItem()
{
	FHitResult Hit;

	if (FindGhostItemPlacementLocation(Hit) && CarriedItem != nullptr)
	{
		DrawDebugSphere(GetWorld(), Hit.Location, 10.0f, 8, FColor::Blue, false, 5.0f);
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 8, FColor::Red, false, 5.0f);
		GhostItem = GetWorld()->SpawnActor<APlaceableItem>(Hit.ImpactPoint, FRotator());
		//UE_LOG(LogTemp, Warning, TEXT("Static mesh is %s"), *CarriedItem->GetStaticMesh()->GetName());
		GhostItem->SetItemStaticMesh(CarriedItem->GetStaticMesh());
		GhostItem->SetActorScale3D(CarriedItem->GetActorScale3D());
		//TODO give the ghost item a transparent material;
	}
}

void UItemPlacementComponent::UpdateGhostItemLocation()
{
	FHitResult Hit;
	if (FindGhostItemPlacementLocation(Hit) && GhostItem != nullptr)
	{
		GhostItem->SetActorLocation(Hit.Location);
		DrawDebugSphere(GetWorld(), Hit.Location, 10.0f, 8, FColor::Blue, false, 5.0f);
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 8, FColor::Red, false, 5.0f);
	}
}

void UItemPlacementComponent::FinishPlacingItem()
{
	FTransform NewTransform = GhostItem->GetActorTransform();
	//FVector GhostItem->GetActorLocation();
	GhostItem->Destroy();
	GhostItem = nullptr;
	CarriedItem->SetActorTransform(NewTransform);
	CarriedItem = nullptr;
	if (GhostItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("That worked"));
	}
}

