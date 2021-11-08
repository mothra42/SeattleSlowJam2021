// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableItem.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APlaceableItem::APlaceableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlaceableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlaceableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlaceableItem::RotateRight(bool bRotateRight)
{
	FRotator NewObjectRotation;
	if (bRotateRight)
	{
		//rotate item to the right by 90 degrees
		NewObjectRotation = GetActorRotation() + FRotator(0.0f, RotationAmountDegrees, 0.0f);
	}
	else
	{
		//rotate item to the left by 90 degrees
		NewObjectRotation = GetActorRotation() + FRotator(0.0f, -RotationAmountDegrees, 0.0f);
	}

	SetActorRotation(NewObjectRotation);
}

void APlaceableItem::AdjustHeight()
{
	if (bCanBePlacedOnWall)
	{
		//Allow player to adjust height to a certain limit
	}
}

void APlaceableItem::PlaceItem()
{
	if (bIsCarried)
	{
		//if the item is carried by the player they can put it down in front of them
		//or on a grid, undetermined.
	}
}
