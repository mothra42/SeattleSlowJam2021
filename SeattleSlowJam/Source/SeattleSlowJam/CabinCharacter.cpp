// Copyright Epic Games, Inc. All Rights Reserved.

#include "CabinCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerComponents/ItemPlacementComponent.h"
#include "CabinItems/PlaceableItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/InputSettings.h"
#include "ScriptingActors/ItemTeleportationArea.h"
#include "PlayerComponents/YarnBallProjectile.h"
#include "NPC/FireGhostEnemy.h"
#include "NPC/GhostEnemy.h"
//////////////////////////////////////////////////////////////////////////
// ACabinCharacter

ACabinCharacter::ACabinCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//ItemPlacement
	ItemPlacementComponent = CreateDefaultSubobject<UItemPlacementComponent>(TEXT("ItemPlacementComponent"));
	SphereTraceOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("SphereTraceOrigin"));
	SphereTraceOrigin->SetupAttachment(RootComponent);
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	JumpBox = CreateDefaultSubobject<UBoxComponent>(TEXT("JumpBox"));
	JumpBox->SetupAttachment(RootComponent);
	JumpBox->OnComponentBeginOverlap.AddDynamic(this, &ACabinCharacter::OnJumpBoxBeginOverlap);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACabinCharacter::OnBeginOverlap);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACabinCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	FInputAxisBinding MoveForward = PlayerInputComponent->BindAxis("MoveForward", this, &ACabinCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACabinCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &ACabinCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACabinCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ACabinCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACabinCharacter::LookUpAtRate);

	//ItemPlacement controls
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACabinCharacter::Interact);
	PlayerInputComponent->BindAction("EnterItemAdjustmentMode", IE_Pressed, this, &ACabinCharacter::EnterItemAdjustmentMode);
	PlayerInputComponent->BindAction("EnterItemAdjustmentMode", IE_Released, this, &ACabinCharacter::ExitItemAdjustmentMode);
	PlayerInputComponent->BindAxis("RotateRight", this, &ACabinCharacter::RotateItem);
	PlayerInputComponent->BindAxis("AdjustLineTraceLength", this, &ACabinCharacter::AdjustItemLineTraceLength);

	//Emergency respawn
	PlayerInputComponent->BindAction("Respawn", IE_Pressed, this, &ACabinCharacter::HandleDeath);

	//Projectile
	PlayerInputComponent->BindAction("FireProjectile", IE_Pressed, this, &ACabinCharacter::ThrowYarnBall);
}

void ACabinCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Set Key mapping in case it was not reset.
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	FKey WKey = FKey(TEXT("W"));
	FKey EKey = FKey(TEXT("E"));
	FKey LMBKey = FKey(TEXT("LeftMouseButton"));
	Settings->RemoveActionMapping(FInputActionKeyMapping(TEXT("Jump"), WKey));
	Settings->AddAxisMapping(FInputAxisKeyMapping(TEXT("MoveForward"), WKey));
	Settings->RemoveActionMapping(FInputActionKeyMapping(TEXT("FireProjectile"), EKey));
	Settings->AddActionMapping(FInputActionKeyMapping(TEXT("Interact"), EKey));
	Settings->RemoveActionMapping(FInputActionKeyMapping(TEXT("FireProjectile"), LMBKey));
	Settings->AddActionMapping(FInputActionKeyMapping(TEXT("Interact"), LMBKey));
}

void ACabinCharacter::AddControllerYawInput(float Value)
{
	//regular controls
	if (!bIsItemAdjustmentMode)
	{
		Super::AddControllerYawInput(Value);
	}
	else
	{
		//rotate item here
		RotateItem(Value);
	}
}

void ACabinCharacter::TurnAtRate(float Rate)
{
	//regular controls
	if (!bIsItemAdjustmentMode)
	{
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
	else
	{
		//rotate item here
		RotateItem(Rate);
	}
}

void ACabinCharacter::AddControllerPitchInput(float Value)
{
	//regular controls
	if (!bIsItemAdjustmentMode)
	{
		Super::AddControllerPitchInput(Value);
	}
}

void ACabinCharacter::LookUpAtRate(float Rate)
{
	if (!bIsItemAdjustmentMode)
	{
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void ACabinCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && !bIsMovementConstrained)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACabinCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ACabinCharacter::Interact()
{
	if (ItemPlacementComponent->GetCarriedItem() != nullptr)
	{
		PlaceItem();
		MoveCameraBack_BP();
	}
	else
	{
		PickupItem();
	}
}

// --------------------------------------------- Item Placement ------------------------------------------
void ACabinCharacter::PickupItem()
{
	//FHitResult Hit;
	TArray<FHitResult> HitResults;
	//TODO for now should just make a slot on the skeletal mesh and attach the item to it.
	//also handle calling methods in ItemPlacementComponent	
	if (SweepForPlaceableItem(HitResults))
	{
		//Will factor this out if I like it.
		auto ClosestActor = HitResults[0].Actor;
		for (FHitResult Hit : HitResults)
		{
			float CurrentClosestDistance = (SphereTraceOrigin->GetComponentLocation() - ClosestActor->GetActorLocation()).Size();
			float NewDistance = (SphereTraceOrigin->GetComponentLocation() - Hit.Location).Size();
			if (NewDistance < CurrentClosestDistance)
			{
				ClosestActor = Hit.Actor;
			}
		}
		APlaceableItem* ItemToCarry = Cast<APlaceableItem>(ClosestActor);
		if (ItemToCarry != nullptr)
		{
			TSubclassOf<APlaceableItem> Class = ItemToCarry->GetClass();
			UE_LOG(LogTemp, Warning, TEXT("Carried Item from character class is %s"), *Class->GetName());
			ItemPlacementComponent->SetCarriedItem(ItemToCarry);
			ItemToCarry->GetStaticMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
			ItemToCarry->SetActorHiddenInGame(true);
			ItemPlacementComponent->SpawnGhostItem();
			MoveCameraOut_BP();
		}
	}
}

bool ACabinCharacter::SweepForPlaceableItem(TArray<FHitResult>& Hits)
{
	FVector TraceBegin = SphereTraceOrigin->GetComponentLocation();
	FVector TraceEnd = SphereTraceOrigin->GetComponentLocation() + FVector::DownVector * 0.01f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { EObjectTypeQuery::ObjectTypeQuery7 };
	return UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), TraceBegin, TraceEnd,
		75.0f, ObjectTypes, false, TArray<AActor*>(), EDrawDebugTrace::None, Hits, true);
	//return UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), TraceBegin, TraceEnd,
	//	75.0f, ObjectTypes, false, TArray<AActor*>(), EDrawDebugTrace::None, Hit, true);
}

void ACabinCharacter::PlaceItem()
{
	ItemPlacementComponent->FinishPlacingItem();
}

void ACabinCharacter::EnterItemAdjustmentMode()
{
	if (ItemPlacementComponent->GetCarriedItem() != nullptr)
	{
		bIsItemAdjustmentMode = true;
	}
}

void ACabinCharacter::ExitItemAdjustmentMode()
{
	bIsItemAdjustmentMode = false;
}

void ACabinCharacter::RotateItem(float Value)
{
	if (bCanRotateItem && Value != 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_RotateItemTimerExpired, 
			this, 
			&ACabinCharacter::RotateTimerExpired, ItemRotationRate
		);

		bCanRotateItem = false;
		ItemPlacementComponent->RotateItem(Value > 0.0f);
	}
}

void ACabinCharacter::RotateTimerExpired()
{
	bCanRotateItem = true;
}

void ACabinCharacter::AdjustItemLineTraceLength(float Value)
{
	if (Value != 0.0f && !bIsItemAdjustmentMode)
	{
		ItemPlacementComponent->AdjustLineTraceLength(Value > 0.0f);
	}
	else if (Value != 0.0f && bIsItemAdjustmentMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Working"));
		ItemPlacementComponent->MoveItemUp(Value);
	}
}

void ACabinCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	TryTeleportItemToBasement(OtherActor);
	AFireGhostEnemy* FireGhost = Cast<AFireGhostEnemy>(OtherActor);
	AGhostEnemy* Ghost = Cast<AGhostEnemy>(OtherActor);
	if (FireGhost != nullptr || Ghost != nullptr)
	{
		HandleDeath();
	}
	
}

void ACabinCharacter::TryTeleportItemToBasement(AActor* ActorToTeleport)
{
	APlaceableItem* ItemToTeleport = Cast<APlaceableItem>(ActorToTeleport) ;
	if (ItemToTeleport != nullptr && !ItemToTeleport->bIsPreparedToTeleport && bIsMovementConstrained && TeleportationArea != nullptr)
	{
		//ItemToTeleport->PrepareToTeleportToBasement(TeleportationArea);
		TeleportationArea->AddItemToTeleport(ItemToTeleport);
		ItemToTeleport->SetActorHiddenInGame(true);
	}
}

//--------------------------------- Character Mode Switch -------------------------------------------
void ACabinCharacter::ShouldConstrainMovement(bool bShouldConstrainMovement)
{
	bIsMovementConstrained = bShouldConstrainMovement;
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	FKey WKey = FKey(TEXT("W"));
	FKey EKey = FKey(TEXT("E"));
	FKey LMBKey = FKey(TEXT("LeftMouseButton"));
	if (bShouldConstrainMovement)
	{
		//remap 'W' to Jump
		Settings->RemoveAxisMapping(FInputAxisKeyMapping(TEXT("MoveForward"), WKey));
		Settings->AddActionMapping(FInputActionKeyMapping(TEXT("Jump"), WKey));

		//remap 'E' to Fire Projectile
		Settings->RemoveActionMapping(FInputActionKeyMapping(TEXT("Interact"), EKey));
		Settings->AddActionMapping(FInputActionKeyMapping(TEXT("FireProjectile"), EKey));
		Settings->RemoveActionMapping(FInputActionKeyMapping(TEXT("Interact"), LMBKey));
		Settings->AddActionMapping(FInputActionKeyMapping(TEXT("FireProjectile"), LMBKey));
		
	}
	else
	{
		//remap 'W' To MoveForward
		Settings->RemoveActionMapping(FInputActionKeyMapping(TEXT("Jump"), WKey));
		Settings->AddAxisMapping(FInputAxisKeyMapping(TEXT("MoveForward"), WKey));

		//remap 'E' to Interact
		Settings->RemoveActionMapping(FInputActionKeyMapping(TEXT("FireProjectile"), EKey));
		Settings->AddActionMapping(FInputActionKeyMapping(TEXT("Interact"), EKey));
		Settings->RemoveActionMapping(FInputActionKeyMapping(TEXT("FireProjectile"), LMBKey));
		Settings->AddActionMapping(FInputActionKeyMapping(TEXT("Interact"), LMBKey));
	}
}

void ACabinCharacter::HandleDeath()
{
	if (bIsMovementConstrained)
	{
		TeleportTo(PlayerRespawnLocation, GetActorRotation());
	}
}

void ACabinCharacter::SetPlayerRespawnLocation(FVector LocationToRespawnAt)
{
	PlayerRespawnLocation = LocationToRespawnAt;
}

//--------------------Attacks---------------------------------------------------------

void ACabinCharacter::ThrowYarnBall()
{
	if (!bIsThrowing)
	{
		PlayThowAnimationMontage();
		bIsThrowing = true;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_ThrowBall,
			this,
			&ACabinCharacter::EndThrowAnimation,
			ThrowBallTimer
		);
	}
}

void ACabinCharacter::EndThrowAnimation()
{
	GetMesh()->GetRightVector();
	FRotator FacingRotation(0, 0, 0);
	//Spawn yarn ball projectile
	if ((GetMesh()->GetComponentRotation().Yaw >= -90.0f && GetMesh()->GetComponentRotation().Yaw < 0))
	{
		FacingRotation = FRotator(0, 0, 0);
	}
	else
	{
		FacingRotation = FRotator(0, 180, 0);
	}
	const FVector SpawnLocation = GetActorLocation() + (GetMesh()->GetRightVector() * 75.0f);
	GetWorld()->SpawnActor<AYarnBallProjectile>(YarnBallClass, SpawnLocation, FacingRotation);
	bIsThrowing = false;
}

void ACabinCharacter::OnJumpBoxBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	
	AFireGhostEnemy* FireGhost = Cast<AFireGhostEnemy>(OtherActor);
	AGhostEnemy* Ghost = Cast<AGhostEnemy>(OtherActor);
	if (FireGhost != nullptr)
	{
		FireGhost->Destroy();
	}
	else if (Ghost != nullptr)
	{
		FVector ForceDirection = (-Ghost->GetActorLocation() + GetActorLocation()).GetSafeNormal();
		GetCharacterMovement()->AddImpulse(ForceDirection * 100000.0f);
	}
}