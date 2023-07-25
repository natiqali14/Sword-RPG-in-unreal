// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RPG_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = true, BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = true))
	UTexture2D* EnemyImage;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = true))
	FString Level;
public:
	void SetHealthBar(float HealthPercent);
	FORCEINLINE void SetLevel(FString Lvl){Level = Lvl;}
	UFUNCTION(BlueprintImplementableEvent)
	void OnEnemyDeath();
	
};
