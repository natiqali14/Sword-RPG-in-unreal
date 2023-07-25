// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuroraController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RPG_API AAuroraController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuroraController();
	virtual void BeginPlay() override;
	
};
