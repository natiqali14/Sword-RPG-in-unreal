// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHud.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RPG_API AMainHud : public AHUD
{
	GENERATED_BODY()
private:
	/** Crosshair for player's main hud */
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Textures | Crosshair",meta = (AllowPrivateAccess = true))
	UTexture2D* Crosshair; // A dot.
	/** Crosshair Screen placement */
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Textures | Crosshair",meta = (AllowPrivateAccess = true))
	FVector2D XYPlane;
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Widgets | Crosshair",meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> MainHUDBP;
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category = "Widgets | Crosshair",meta = (AllowPrivateAccess = true))
	UUserWidget* MainHUD;
	
	
public:
	AMainHud();
	virtual void DrawHUD() override;
	void DrawCrosshair();
	virtual void BeginPlay() override;
	 
	
};

