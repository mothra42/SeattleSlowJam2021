// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPlacementComponent.h"
#include "../CabinItems/PlaceableItem.h"
#include "Camera/CameraComponent.h"
#include "../CabinCharacter.h"
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

	if (FindGhostItemPlacementLocation(Hit) && CarriedItem != nullptr)
	{
		FRotator CarriedItemRotation = CarriedItem->GetActorRotation();
		GhostItem = GetWorld()->SpawnActor<APlaceableItem>(Hit.ImpactPoint, CarriedItemRotation);
		GhostItem->GetStaticMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		GhostItem->SetItemStaticMesh(CarriedItem->GetStaticMesh());
		GhostItem->GetStaticMesh()->SetRelativeScale3D(CarriedItem->GetStaticMesh()->GetRelativeScale3D());
		//TODO give the ghost item a transparent material;
	}
}

void UItemPlacementComponent::UpdateGhostItemLocation()
{
	FHitResult Hit;
	FHitResult SweepHit;
	bool bDidActorMove;
	if (FindGhostItemPlacementLocation(Hit) && GhostItem != nullptr)
	{
		bDidActorMove = GhostItem->SetActorLocation(Hit.Location, true, &SweepHit, ETeleportType::None);
		//if (!bDidActorMove)
		//{
		//	//TODO do some checks to see which Coordinates need to be preserved
		//	FVector ComparisonVector = (GhostItem->GetActorLocation() - SweepHit.ImpactPoint).GetAbs();
		//	if (ComparisonVector.X > 0.0001)
		//	{
		//		FVector MoveToLocation = FVector(GhostItem->GetActorLocation().X, Hit.Location.Y, Hit.Location.Z);
		//		UE_LOG(LogTemp, Warning, TEXT("Keep X the same"));
		//		GhostItem->SetActorLocation(MoveToLocation);
		//	}
		//	else if (ComparisonVector.Y > 0.0001)
		//	{
		//		FVector MoveToLocation = FVector(Hit.Location.X, GhostItem->GetActorLocation().Y, Hit.Location.Z);
		//		UE_LOG(LogTemp, Warning, TEXT("Keep Y the same"));
		//		GhostItem->SetActorLocation(MoveToLocation);
		//	}
		//	else
		//	{
		//		FVector MoveToLocation = FVector(Hit.Location.X, Hit.Location.Y, GhostItem->GetActorLocation().Z);
		//		UE_LOG(LogTemp, Warning, TEXT("Keep Z the same"));
		//		GhostItem->SetActorLocation(MoveToLocation);
		//	}
		//}
	}
	else
	{
		//TODO Apply a color that makes it clear it's an invalid placement.
		ACabinCharacter* PlayerCharacter = Cast<ACabinCharacter>(GetOwner());
		FVector StartLocation = PlayerCharacter->GetFollowCamera()->GetComponentLocation();
		FVector EndLocation = StartLocation + PlayerCharacter->GetFollowCamera()->GetForwardVector() * LineTraceLength;
		bDidActorMove = GhostItem->SetActorLocation(EndLocation, true, &SweepHit, ETeleportType::None);
		if (SweepHit.Actor != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit is at %s"), *SweepHit.Actor->GetName());

			//if (!bDidActorMove)
			//{
			//	//TODO do some checks to see which Coordinates need to be preserved
			//	FVector ComparisonVector = (GhostItem->GetActorLocation() - SweepHit.ImpactPoint).GetAbs();
			//	if (ComparisonVector.X > 0.0001)
			//	{
			//		FVector MoveToLocation = FVector(GhostItem->GetActorLocation().X, Hit.Location.Y, Hit.Location.Z);
			//		UE_LOG(LogTemp, Warning, TEXT("Keep X the same"));
			//		GhostItem->SetActorLocation(MoveToLocation);
			//	}
			//	else if (ComparisonVector.Y > 0.0001)
			//	{
			//		FVector MoveToLocation = FVector(Hit.Location.X, GhostItem->GetActorLocation().Y, Hit.Location.Z);
			//		UE_LOG(LogTemp, Warning, TEXT("Keep Y the same"));
			//		GhostItem->SetActorLocation(MoveToLocation);
			//	}
			//	else
			//	{
			//		FVector MoveToLocation = FVector(Hit.Location.X, Hit.Location.Y, GhostItem->GetActorLocation().Z);
			//		UE_LOG(LogTemp, Warning, TEXT("Keep Z the same"));
			//		GhostItem->SetActorLocation(MoveToLocation);
			//	}
			//}
		}
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
}

void UItemPlacementComponent::RotateItem(bool bIsRightRotation)
{
	if (CarriedItem != nullptr && GhostItem != nullptr)
	{
		GhostItem->RotateRight(bIsRightRotation);
	}
}