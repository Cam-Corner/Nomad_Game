// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "StrategyCameraPawn.generated.h"

UENUM(BlueprintType)
enum ECameraMovementStyle
{
	ECMS_None  			UMETA("None"),
	ECMS_Scrolling  	UMETA("Scrolling"),
	ECMS_Flying 		UMETA("Flying"),
};

/**
 * 
 */
UCLASS()
class NOMAD_GAME_API AStrategyCameraPawn : public APawn
{
	GENERATED_BODY()
	
public:
	AStrategyCameraPawn();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;
protected:
	/** Handles the scrolling style camera movement. Gets called internally from AStrategySpectatorPawn 
	Params:
	ForwardAxisDirValue & RightAxisDirValue: will be normalized from a vector2D  and passed into it
	ForwardAxisDirValue & RightAxisDirValue: can be positive or negative.
	This is for the scrolling Camera. Scrolling Camera is the camera that moves parallel to the worlds axis and not where the camera is facing,
	use flying Camera for movement based on where it is facing.
	*/
	virtual void HandleScrollingCameraMovement(const float DeltaTime, const float ForwardAxisDirValue, const float RightAxisDirValue);


	/** Handles the scrolling style camera zoom in and out. Gets called internall from AStrategySpectatorPawn
	Params:
	ZoomAxis: will be normalized from a vector 3 and passed into it
	ZoomAxis: can be positive or negative.
	This is for the scrolling Camera. Scrolling Camera is the camera that moves parallel to the worlds axis and not where the camera is facing,
	use flying Camera for movement based on where it is facing */
	virtual void HandleScrollingCameraZoom(const float DeltaTime, const float ZoomAxis);

	/** Handles the flying style camera movement. Gets called internally from AStrategySpectatorPawn
	Params:
	ForwardAxisDirValue & RightAxisDirValue: will be normalized from a vector2D  and passed into it
	ForwardAxisDirValue & RightAxisDirValue: can be positive or negative.
	This is for the flying Camera. Flying Camera is a camera that moves based on where the camera is facing. For a camera that moves parrallel to the world,
	use a scrolling style camera.*/
	virtual void HandleFlyingCameraMovement(const float DeltaTime, const float ForwardAxisDirValue, const float RightAxisDirValue);


	/** Handles the camera zoom in and out. Gets called internall from AStrategySpectatorPawn
	Params:
	ZoomAxis: will be normalized from a vector 3 and passed into it
	ZoomAxis: can be positive or negative.
	This is for the flying Camera. Flying Camera is a camera that moves based on where the camera is facing. For a camera that moves parrallel to the world,
	use a scrolling style camera.*/
	virtual void HandleFlyingCameraZoom(const float DeltaTime, const float ZoomAxis);

	/* Handles the final rotation of the camera from the input recieved */
	virtual void HandleCameraRotation(const float DeltaTime, const float PitchAxisInput, const float YawAxisInput);

	/* Set camera movement style */
	virtual void SetCameraMovementStyle(ECameraMovementStyle CameraStyle);

	/* add forward movement input for it to be handle by this class. Will get normalized before use */
	void AddForwardMovementInput(float ForwardDir);

	/* add right movement input for it to be handle by this class. Will get normalized before use */
	void AddRightMovementInput(float RightDir);

	/* add camera pitch rotation */
	void AddPitchRotation(float PitchAmount);

	/*  add camera yaw rotation */
	void AddYawRotation(float YawAmount);

	/* add movement input for it to be handle by this class. Will get normalized before use */
	void AddZoomInput(float ZoomAmount);

private:
	UFUNCTION()
		void MoveForwardInput(float Value);

	UFUNCTION()
		void MoveRightInput(float Value);

	UFUNCTION()
		void CameraZoomInput(float Value);

	UFUNCTION()
		void CameraStyleTogglePressed();

	UFUNCTION()
		void CameraStyleToggleReleased();

	UFUNCTION()
		void PitchInput(float Value);

	UFUNCTION()
		void YawInput(float Value);

	void HandleMovement(float DeltaTime);

	void HandleCameraZoom(float DeltaTime);

	void HandleRotation(float DeltaTime);

	/* Consume the movement input this frame and reset it back to 0 for use in next frame */
	FVector2D ConsumeMovementInput();

	/* Consume the Zoom input this frame and reset it back to 0 for use in next frame */
	float ConsumeZoomInput();

	/* Consume the rotation input this frame and reset it back to 0 for use in next frame */
	FVector2D ConsumeRotationInput();

	/* set where the camera is allowed to rotate or not */
	void CameraAllowedToRotate(bool bCanRotate);
protected:
	class UCameraComponent* _CameraComp;

private:
	FVector2D _MovementInputToConsume = FVector2D::ZeroVector;
	float _CameraZoomInputToConsume = 0;
	FVector2D _CameraRotationInputToConsume = FVector2D::ZeroVector;

	ECameraMovementStyle _CameraMovementStyle = ECameraMovementStyle::ECMS_Scrolling;

	bool _bCameraCanRotate = false;
protected:
	UPROPERTY(EditAnywhere)
	float _CameraMoveSpeed = 3000.f;

	UPROPERTY(EditAnywhere)
	float _CameraZoomSpeed = 5000.f;

	UPROPERTY(EditAnywhere)
	float _CameraSensitivity = 100.f;

	UPROPERTY(EditAnywhere)
	float _CameraPitchLock = 85.f;
};
