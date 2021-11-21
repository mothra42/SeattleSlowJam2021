// Fill out your copyright notice in the Description page of Project Settings.


#include "FireGhostAIController.h"
#include "Kismet/GameplayStatics.h"
#include "../FireGhostEnemy.h"
#include "Kismet/KismetMathLibrary.h"

void AFireGhostAIController::BeginPlay()
{
	Super::BeginPlay();

	//Set timer to check if player is within acceptable radius
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_CheckForPlayerTimerExpired,
		this,
		&AFireGhostAIController::CheckIfPlayerInRadius,
		PlayerCheckTimer,
		true
	);

	//set timer to update the player's location for pursuit.
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_UpdatePlayerLocationTimerExpired,
		this,
		&AFireGhostAIController::UpdateMoveTo,
		UpdatePlayerLocationTimer,
		true
	);
}

void AFireGhostAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFireGhostAIController::CheckIfPlayerInRadius()
{
	AFireGhostEnemy* ControlledFireGhost = Cast<AFireGhostEnemy>(GetPawn());
	FVector PlayerLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	FVector ControlledPawnLocation = ControlledFireGhost->GetActorLocation();
	float Distance = FMath::Abs((PlayerLocation - ControlledPawnLocation).Size());
	if (Distance <= AcceptablePlayerRadius)
	{
		bShouldPursuePlayer = true;
		ControlledFireGhost->SetActorTickEnabled(false);
	}
	else
	{
		bShouldPursuePlayer = false;
		ControlledFireGhost->SetActorTickEnabled(true);
	}
}

void AFireGhostAIController::PursuePlayer()
{
	if (bShouldPursuePlayer)
	{
		
		AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(
			GetPawn()->GetActorLocation(), 
			Player->GetActorLocation()
		);
		MoveToActor(Player);
		GetPawn()->SetActorRotation(FRotator(0, NewRotation.Yaw - 90, 0) );
	}
}

void AFireGhostAIController::UpdateMoveTo()
{
	PursuePlayer();
}
