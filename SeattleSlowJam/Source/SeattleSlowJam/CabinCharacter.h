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
	class USceneComponent* ItemAtachmentComponent;
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

protected:
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

private:
	bool bIsItemAdjustmentMode = false;

	bool SweepForPlaceableItem(FHitResult& Hit);

	bool bCanRotateItem = true;

	FTimerHandle TimerHandle_RotateItemTimerExpired;

	void EnterItemAdjustmentMode();

	void ExitItemAdjustmentMode();

	void RotateTimerExpired();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemInteraction, meta = (AllowPrivateAccess = "true"))
	float RotationRate = 0.3f;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
private:
	bool bIsMovementConstrained = false;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

