// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Project_RPG/Items/IInteractable.h"
#include "InteractionComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_RPG_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class AAurora;
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = PROPS,meta = (AllowPrivateAccess = true))
	bool bCanRayCast;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = PROPS,meta = (AllowPrivateAccess = true))
	AActor* ActorUnderCrosshair;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = PROPS,meta = (AllowPrivateAccess = true))
	AAurora* Player;
	// UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = InteractableItems, meta = (AllowPrivateAccess = true))
	// TSet<IIInteractable*> Interactables;
	//
	
public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	AActor* GetActorThroughRayCast();
	void ActorInteraction();
	void Interact();

	FORCEINLINE void SetCanRayCast(bool bRayCast){bCanRayCast = bRayCast;}
	FORCEINLINE bool GetCanRayCast(){return bCanRayCast;}
	
private:
	void SetActorUnderCrosshair(AActor* ActorUnderTrace);
	void ShowIntractableInfo(AActor* IntractableActor,bool bShow);
	
		
};
