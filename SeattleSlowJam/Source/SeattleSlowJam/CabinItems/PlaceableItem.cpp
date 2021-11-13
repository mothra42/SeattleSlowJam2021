// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
APlaceableItem::APlaceableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//RootComponent = Root;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;
	//ItemMesh->SetupAttachment(RootComponent);
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
	UE_LOG(LogTemp, Warning, TEXT("I Exist"));
}

void APlaceableItem::RotateRight(bool bIsRightRotation)
{
	FRotator NewObjectRotation;
	if (bIsRightRotation)
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

void APlaceableItem::TeleportToBasement()
{
	SetActorLocation(BasementTeleportLocation, true);
}
