// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "../ScriptingActors/ItemTeleportationArea.h"

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

void APlaceableItem::AdjustHeight(bool bIsUp)
{
	if (bCanBePlacedOnWall)
	{
		FVector NewLocation;
		if (bIsUp)
		{
			NewLocation = GetActorLocation() + (FVector::UpVector * ZMovementStepSize);
		}
		else
		{
			NewLocation = GetActorLocation() - (FVector::UpVector * ZMovementStepSize);
		}
		SetActorLocation(NewLocation, true);
	}
}

void APlaceableItem::PrepareToTeleportToBasement(ATeleportationArea* TeleportationArea)
{
	//TeleportationArea->AddItemToTeleport(this);
}

void APlaceableItem::TeleportToBasement()
{
	SetActorLocation(BasementTeleportLocation);
	//if (TeleportationArea != nullptr)
	//{
	//	TeleportationArea->CheckShouldPortalDoorOpen();
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("PlaceableItem %s, has no set teleportation area!"), *GetName());
	//}
}

void APlaceableItem::ChangeCollisionResponse(ECollisionResponse ResponseToSet)
{
	TInlineComponentArray<UStaticMeshComponent*> AllStaticMeshes;
	GetComponents(AllStaticMeshes);
	for (UStaticMeshComponent* Comp : AllStaticMeshes)
	{
		Comp->SetCollisionResponseToAllChannels(ResponseToSet);
	}
}