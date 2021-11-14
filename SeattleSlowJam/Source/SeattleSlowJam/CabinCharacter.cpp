// Copyright Epic Games, Inc. All Rights Reserved.

#include "CabinCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerComponents/ItemPlacementComponent.h"
#include "CabinItems/PlaceableItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/SkeletalMeshSocket.h"

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
	ItemAtachmentComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ItemAttachmentComponent"));
	ItemAtachmentComponent->SetupAttachment(RootComponent);
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

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

	PlayerInputComponent->BindAxis("MoveForward", this, &ACabinCharacter::MoveForward);
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
	}
	else
	{
		PickupItem();
	}
}

// --------------------------------------------- Item Placement ------------------------------------------
void ACabinCharacter::PickupItem()
{
	FHitResult Hit;
	//TODO for now should just make a slot on the skeletal mesh and attach the item to it.
	//also handle calling methods in ItemPlacementComponent	
	if (SweepForPlaceableItem(Hit))
	{
		APlaceableItem* ItemToCarry = Cast<APlaceableItem>(Hit.Actor);
		if (ItemToCarry != nullptr)
		{
			ItemPlacementComponent->SetCarriedItem(Cast<APlaceableItem>(Hit.Actor));
			ItemToCarry->GetStaticMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
			ItemToCarry->SetActorHiddenInGame(true);
			ItemPlacementComponent->SpawnGhostItem();
		}
	}
}

bool ACabinCharacter::SweepForPlaceableItem(FHitResult& Hit)
{
	FVector TraceBegin = GetActorLocation() + FVector::DownVector * 75.0f;
	FVector TraceEnd = GetActorLocation() + FVector::DownVector * 75.01f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { EObjectTypeQuery::ObjectTypeQuery7 };
	return UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), TraceBegin, TraceEnd,
		75.0f, ObjectTypes, false, TArray<AActor*>(), EDrawDebugTrace::None, Hit, true);
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
}

void ACabinCharacter::TryTeleportItemToBasement(AActor* ActorToTeleport)
{
	APlaceableItem* ItemToTeleport = Cast<APlaceableItem>(ActorToTeleport) ;
	if (ItemToTeleport != nullptr && !ItemToTeleport->bIsPreparedToTeleport && bIsMovementConstrained)
	{
		ItemToTeleport->PrepareToTeleportToBasement();
		ItemToTeleport->SetActorHiddenInGame(true);
	}
}

//--------------------------------- Character Mode Switch -------------------------------------------
void ACabinCharacter::ShouldConstrainMovement(bool bShouldConstrainMovement)
{
	bIsMovementConstrained = bShouldConstrainMovement;
}

//TODO add OnComponentBeginOverlap to the capsule component to check for items in the dungeon.
//if it overlaps one in the dungeon call the teleport method. 