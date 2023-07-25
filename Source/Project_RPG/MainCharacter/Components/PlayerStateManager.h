// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Project_RPG/Enums/CustomStates.h"
#include "PlayerStateManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_RPG_API UPlayerStateManager : public UActorComponent
{
	GENERATED_BODY()
	friend class AAurora;
	UPROPERTY()
	AAurora* PlayerToManage;
	
	
public:	
		UPlayerStateManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
};
