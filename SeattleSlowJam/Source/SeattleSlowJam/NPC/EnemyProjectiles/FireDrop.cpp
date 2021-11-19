// Fill out your copyright notice in the Description page of Project Settings.


#include "FireDrop.h"
#include "Components/SphereComponent.h"

// Sets default values
AFireDrop::AFireDrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

}

// Called when the game starts or when spawned
void AFireDrop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFireDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CollisionComponent->AddForce(FVector::UpVector * 8.0f);
	//TODO make sway sinusoidal on the way down. expire when it hits anything. 
	//seems like it should work well enough.
}

