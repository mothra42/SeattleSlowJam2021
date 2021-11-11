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
	PitchAdjustment = DefaultPitchAdjustment;
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
	//FHitResult Hit;
	//FindGhostItemPlacementLocation(Hit);
}

bool UItemPlacementComponent::FindGhostItemPlacementLocation(FHitResult& Hit)
{
	ACabinCharacter* PlayerCharacter = Cast<ACabinCharacter>(GetOwner());
	FVector StartLocation = PlayerCharacter->GetFollowCamera()->GetComponentLocation();
	FVector EndLocation = StartLocation + PlayerCharacter->GetFollowCamera()->GetForwardVector() * LineTraceLength;
	FRotator ActorRotation = GetOwner()->GetActorRotation();
	FVector TraceDirection = UKismetMathLibrary::CreateVectorFromYawPitch(ActorRotation.Yaw, PitchAdjustment);
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	//TODO consider changing this to a trace by profile
	return GetWorld()->LineTraceSingleByObjectType(Hit,
		StartLocation,
		EndLocation,
		ObjectParams);
}

//This method should only be called when the player first picks up and item;
void UItemPlacementComponent::SpawnGhostItem()
{
	FHitResult Hit;

	if (FindGhostItemPlacementLocation(Hit) && CarriedItem != nullptr)
	{
		GhostItem = GetWorld()->SpawnActor<APlaceableItem>(Hit.ImpactPoint, FRotator());
		GhostItem->SetActorEnableCollision(false);
		GhostItem->SetItemStaticMesh(CarriedItem->GetStaticMesh());
		GhostItem->GetStaticMesh()->SetRelativeScale3D(CarriedItem->GetStaticMesh()->GetRelativeScale3D());
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
		//If There is no hit change location to where the trace ends
		//TODO Apply a color that makes it clear it's an invalid placement.
		FRotator ActorRotation = GetOwner()->GetActorRotation();
		FVector TraceDirection = UKismetMathLibrary::CreateVectorFromYawPitch(ActorRotation.Yaw, PitchAdjustment);
		GhostItem->SetActorLocation(GetOwner()->GetActorLocation() + TraceDirection * LineTraceLength);
	}
}

void UItemPlacementComponent::FinishPlacingItem()
{
	FTransform NewTransform = GhostItem->GetActorTransform();
	GhostItem->Destroy();
	GhostItem = nullptr;
	NewTransform.SetScale3D(FVector(1.0, 1.0, 1.0));
	CarriedItem->SetActorTransform(NewTransform);
	CarriedItem->SetActorHiddenInGame(false);
	CarriedItem->SetActorEnableCollision(true);
	CarriedItem = nullptr;
	PitchAdjustment = DefaultPitchAdjustment;
}

void UItemPlacementComponent::AdjustPitchAdjustment(bool bIsAdjustingUp)
{
	if (CarriedItem != nullptr)
	{
		//TODO Clamp these values.
		if (bIsAdjustingUp)
		{
			PitchAdjustment += LineTracePitchAdjustmentAmount;
		}
		else
		{
			PitchAdjustment -= LineTracePitchAdjustmentAmount;
		}
	}
}

void UItemPlacementComponent::RotateItem(bool bIsRightRotation)
{
	if (CarriedItem != nullptr && GhostItem != nullptr)
	{
		GhostItem->RotateRight(bIsRightRotation);
	}
}