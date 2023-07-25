// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MechanicsFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RPG_API UMechanicsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static void MultiBoxTraceByObject(UWorld* World,FVector Start, FVector End,
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes, TArray<FHitResult> &HitResults );
	
};
