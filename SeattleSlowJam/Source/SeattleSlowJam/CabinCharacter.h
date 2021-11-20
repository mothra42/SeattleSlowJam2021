// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CabinCharacter.generated.h"

UCLASS(config=Game)
class ACabinCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemInteraction, meta = (AllowPrivateAccess = "true"))
	class UItemPlacementComponent* ItemPlacementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemInteraction, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* SphereTraceOrigin;
public:
	ACabinCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UFUNCTION(BlueprintCallable)
	void ShouldConstrainMovement(bool bShouldConstrainMovement);

	UFUNCTION(BlueprintCallable)
	void HandleDeath();

	UPROPERTY(Category = "TeleportSettings", EditAnywhere, BlueprintReadWrite)
	class AItemTeleportationArea* TeleportationArea;

	UFUNCTION(BlueprintCallable)
	void SetPlayerRespawnLocation(FVector LocationToRespawnAt);

protected:

	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	virtual void AddControllerYawInput(float Value) override;
	virtual void AddControllerPitchInput(float Value) override;

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void Interact();

	void PickupItem();

	void PlaceItem();

	void RotateItem(float Value);

	void AdjustItemLineTraceLength(float Value);

private:
	bool bIsItemAdjustmentMode = false;

	//bool SweepForPlaceableItem(FHitResult& Hit);
	bool SweepForPlaceableItem(TArray<FHitResult>& Hits);

	bool bCanRotateItem = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ItemInteraction, meta = (AllowPrivateAccess = "true"))
	float ItemRotationRate = 0.1f;

	FTimerHandle TimerHandle_RotateItemTimerExpired;

	void EnterItemAdjustmentMode();

	void ExitItemAdjustmentMode();

	void RotateTimerExpired();

	void TryTeleportItemToBasement(AActor* ActorToTeleport);

	FVector PlayerRespawnLocation;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
private:
	bool bIsMovementConstrained = false;

public:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE bool GetItemModeState() const { return bIsItemAdjustmentMode; }
};

