// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateManager.h"


UPlayerStateManager::UPlayerStateManager()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UPlayerStateManager::BeginPlay()
{
	Super::BeginPlay();

	
	
}


// Called every frame
void UPlayerStateManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

