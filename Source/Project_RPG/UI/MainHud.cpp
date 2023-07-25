// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHud.h"

#include "Blueprint/UserWidget.h"


AMainHud::AMainHud()
{
	XYPlane = FVector2D(0.f,0.f);
}

void AMainHud::DrawHUD()
{
	Super::DrawHUD();
	//DrawCrosshair();
	
}

void AMainHud::DrawCrosshair()  // Redundant function
{
	if(!Crosshair) return;
	FVector2D ViewPort;
	if(!GEngine) return;;
	GEngine->GameViewport->GetViewportSize(ViewPort);
	FVector2D CenterPoint = {ViewPort.X / 2.f,ViewPort.Y/2.f};
	const float CrosshairX = Crosshair->GetSizeX();
	const float CrosshairY = Crosshair->GetSizeY();
	FVector2D DrawPoint {CenterPoint.X - CrosshairX + XYPlane.X, CenterPoint.Y - CrosshairY + XYPlane.Y};
	DrawTexture(Crosshair,DrawPoint.X,DrawPoint.Y,CrosshairX,CrosshairY,
			0,0,1,1);
	
	
}

void AMainHud::BeginPlay()
{
	Super::BeginPlay();
	MainHUD = CreateWidget(GetOwningPlayerController(),MainHUDBP);
	MainHUD->AddToViewport();
	MainHUD->SetVisibility(ESlateVisibility::Visible);
}
