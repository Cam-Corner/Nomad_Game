// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePC.generated.h"

/**
 * 
 */
UCLASS()
class NOMAD_GAME_API AGamePC : public APlayerController
{
	GENERATED_BODY()
public:
	AGamePC();

	virtual void SetupInputComponent() override;


};
