// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/StrategyCameraPawn.h"
#include "Camera/CameraComponent.h"

AStrategyCameraPawn::AStrategyCameraPawn()
{
	_CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	RootComponent = _CameraComp;
}

void AStrategyCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (!PlayerInputComponent)
		return;

	//Axis Bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &AStrategyCameraPawn::MoveForwardInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &AStrategyCameraPawn::MoveRightInput);
	PlayerInputComponent->BindAxis("CameraZoom", this, &AStrategyCameraPawn::CameraZoomInput);
	PlayerInputComponent->BindAxis("MouseX", this, &AStrategyCameraPawn::YawInput);
	PlayerInputComponent->BindAxis("MouseY", this, &AStrategyCameraPawn::PitchInput);

	//Action Bindings
	PlayerInputComponent->BindAction("CameraStyleToggle", IE_Pressed, this, &AStrategyCameraPawn::CameraStyleTogglePressed);
	PlayerInputComponent->BindAction("CameraStyleToggle", IE_Released, this, &AStrategyCameraPawn::CameraStyleToggleReleased);
}

void AStrategyCameraPawn::Tick(float DeltaTime)
{
	HandleMovement(DeltaTime);
	HandleCameraZoom(DeltaTime);
	HandleRotation(DeltaTime);
}

void AStrategyCameraPawn::HandleScrollingCameraMovement(float DeltaTime, float ForwardAxisDirValue, const float RightAxisDirValue)
{
	FQuat CurrentQuat = GetActorQuat();
	FQuat YawOffset = FQuat(FRotator(-CurrentQuat.Rotator().Pitch, 0, 0));
	CurrentQuat = CurrentQuat * YawOffset;

	float FinalMoveAmount = _CameraMoveSpeed * DeltaTime;

	FVector FinalOffset = FVector::ZeroVector;
	FinalOffset += CurrentQuat.GetForwardVector() * FinalMoveAmount * ForwardAxisDirValue;
	FinalOffset += CurrentQuat.GetRightVector() * FinalMoveAmount * RightAxisDirValue;

	AddActorWorldOffset(FinalOffset);
}


void AStrategyCameraPawn::HandleScrollingCameraZoom(const float DeltaTime, const float ZoomAxis)
{
	float FinalMoveAmount = _CameraZoomSpeed * DeltaTime * ZoomAxis;

	AddActorWorldOffset(GetActorForwardVector() * FinalMoveAmount);
}

void AStrategyCameraPawn::HandleFlyingCameraMovement(const float DeltaTime, const float ForwardAxisDirValue, const float RightAxisDirValue)
{
	float FinalMoveAmount = _CameraMoveSpeed * DeltaTime;

	FVector FinalOffset = FVector::ZeroVector;
	FinalOffset += GetActorForwardVector() * FinalMoveAmount * ForwardAxisDirValue;
	FinalOffset += GetActorRightVector() * FinalMoveAmount * RightAxisDirValue;

	AddActorWorldOffset(FinalOffset);
}

void AStrategyCameraPawn::HandleFlyingCameraZoom(const float DeltaTime, const float ZoomAxis)
{
	float FinalMoveAmount = _CameraZoomSpeed * DeltaTime * ZoomAxis;

	AddActorWorldOffset(FVector::UpVector * FinalMoveAmount);
}

void AStrategyCameraPawn::HandleCameraRotation(const float DeltaTime, const float PitchAxisInput, const float YawAxisInput)
{
	float CameraSens = _CameraSensitivity * DeltaTime;

	FQuat CurrentQuat = GetActorQuat();
	FRotator CurrentRot = CurrentQuat.Rotator();

	CurrentRot.Yaw += CameraSens * YawAxisInput;
	CurrentRot.Pitch += CameraSens * PitchAxisInput;
	CurrentRot.Pitch = FMath::Clamp(CurrentRot.Pitch, -_CameraPitchLock, _CameraPitchLock);

	SetActorRotation(FQuat(CurrentRot));
}

void AStrategyCameraPawn::SetCameraMovementStyle(ECameraMovementStyle CameraStyle)
{
	_CameraMovementStyle = CameraStyle;
}

void AStrategyCameraPawn::AddForwardMovementInput(float ForwardDir)
{
	_MovementInputToConsume.X = FMath::Clamp(ForwardDir, -1, 1);
}

void AStrategyCameraPawn::AddRightMovementInput(float RightDir)
{
	_MovementInputToConsume.Y = FMath::Clamp(RightDir, -1, 1);
}

void AStrategyCameraPawn::AddPitchRotation(float PitchAmount)
{
	_CameraRotationInputToConsume.Y = PitchAmount;
}

void AStrategyCameraPawn::AddYawRotation(float YawAmount)
{
	_CameraRotationInputToConsume.X = YawAmount;
}

void AStrategyCameraPawn::AddZoomInput(float ZoomAmount)
{
	_CameraZoomInputToConsume = FMath::Clamp(ZoomAmount, -1, 1);
}

void AStrategyCameraPawn::MoveForwardInput(float Value)
{
	AddForwardMovementInput(Value);
}

void AStrategyCameraPawn::MoveRightInput(float Value)
{
	AddRightMovementInput(Value);
}

void AStrategyCameraPawn::CameraZoomInput(float Value)
{
	AddZoomInput(Value);
}

void AStrategyCameraPawn::CameraStyleTogglePressed()
{
	SetCameraMovementStyle(ECameraMovementStyle::ECMS_Flying);
	CameraAllowedToRotate(true);
}

void AStrategyCameraPawn::CameraStyleToggleReleased()
{
	SetCameraMovementStyle(ECameraMovementStyle::ECMS_Scrolling);
	CameraAllowedToRotate(false);
}

void AStrategyCameraPawn::PitchInput(float Value)
{
	AddPitchRotation(Value);
}

void AStrategyCameraPawn::YawInput(float Value)
{
	AddYawRotation(Value);
}

void AStrategyCameraPawn::HandleMovement(float DeltaTime)
{
	FVector2D ConsumedMovementInput = ConsumeMovementInput();

	if (_CameraMovementStyle == ECameraMovementStyle::ECMS_Scrolling)
	{
		HandleScrollingCameraMovement(DeltaTime, ConsumedMovementInput.X, ConsumedMovementInput.Y);
	}
	else if (_CameraMovementStyle == ECameraMovementStyle::ECMS_Flying)
	{
		HandleFlyingCameraMovement(DeltaTime, ConsumedMovementInput.X, ConsumedMovementInput.Y);
	}
}

void AStrategyCameraPawn::HandleCameraZoom(float DeltaTime)
{
	float ConsumedZoomInput = ConsumeZoomInput();

	if (_CameraMovementStyle == ECameraMovementStyle::ECMS_Scrolling)
	{
		HandleScrollingCameraZoom(DeltaTime, ConsumedZoomInput);
	}
	else if (_CameraMovementStyle == ECameraMovementStyle::ECMS_Flying)
	{
		HandleFlyingCameraZoom(DeltaTime, ConsumedZoomInput);
	}
}

void AStrategyCameraPawn::HandleRotation(float DeltaTime)
{
	if (!_bCameraCanRotate)
		return;

	FVector2D RotationConsumed = ConsumeRotationInput();
	
	HandleCameraRotation(DeltaTime, RotationConsumed.Y, RotationConsumed.X);
}

FVector2D AStrategyCameraPawn::ConsumeMovementInput()
{
	FVector2D ConsumedInput = _MovementInputToConsume.GetSafeNormal();
	_MovementInputToConsume = FVector2D::ZeroVector;

	return ConsumedInput;
}

float AStrategyCameraPawn::ConsumeZoomInput()
{
	float ConsumedInput = _CameraZoomInputToConsume;
	_CameraZoomInputToConsume = 0;

	return FMath::Clamp(ConsumedInput, -1, 1);

}

FVector2D AStrategyCameraPawn::ConsumeRotationInput()
{
	FVector2D ConsumedInput = _CameraRotationInputToConsume;
	_CameraRotationInputToConsume = FVector2D::ZeroVector;

	//ConsumedInput.X = FMath::Clamp(ConsumedInput.X, -1, 1);
	//ConsumedInput.Y = FMath::Clamp(ConsumedInput.Y, -1, 1);

	return ConsumedInput;
}

void AStrategyCameraPawn::CameraAllowedToRotate(bool bCanRotate)
{
	//bCanRotate ? _bCameraCanRotate = true : _bCameraCanRotate = false;
	_bCameraCanRotate = bCanRotate;
}
