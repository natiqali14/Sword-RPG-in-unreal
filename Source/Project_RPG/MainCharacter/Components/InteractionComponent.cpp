// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Project_RPG/Items/IInteractable.h"
#include "Project_RPG/Items/Weapons/Weapon.h"
#include "Project_RPG/MainCharacter/Aurora.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bCanRayCast = false;
	ActorUnderCrosshair = nullptr;
	
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ActorInteraction();

	// ...
}

AActor* UInteractionComponent::GetActorThroughRayCast()
{
	if(!bCanRayCast) return nullptr;
	FVector2D ViewPort;
	if(!GEngine) return nullptr;
	GEngine->GameViewport->GetViewportSize(ViewPort);
	FVector2D CenterPoint = {ViewPort.X / 2.f,ViewPort.Y/2.f};
	FVector WorldPos;
	FVector WorldDirection;
	UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetWorld(),0),CenterPoint,
		WorldPos,WorldDirection);
	FVector TraceVector = WorldPos + (WorldDirection * 8000);
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	QueryParams.bDebugQuery = true;
	GetWorld()->LineTraceSingleByChannel(HitResult,WorldPos,TraceVector,
								ECC_Visibility,QueryParams);
	//DrawDebugLine(GetWorld(),WorldPos,TraceVector,FColor::Red );
	if(HitResult.IsValidBlockingHit())
	{
		
		return HitResult.GetActor();
	}
	
	return nullptr;
	
}

void UInteractionComponent::ActorInteraction()
{
	AActor* RayCastedActor = GetActorThroughRayCast();
	SetActorUnderCrosshair(RayCastedActor);
}



void UInteractionComponent::SetActorUnderCrosshair(AActor* ActorUnderTrace)
{
	
	
	if(ActorUnderTrace)
	{
		if(ActorUnderTrace != ActorUnderCrosshair)
		{
			ShowIntractableInfo(ActorUnderCrosshair,false);
		}
		ActorUnderCrosshair = ActorUnderTrace;
		if(Player)
		{
			if(Player->GetOverlappingItems().Contains(Cast<AItem>(ActorUnderCrosshair)))
			{
				ShowIntractableInfo(ActorUnderCrosshair,true);
			}
		}
	
	}
	else
	{
		if(ActorUnderCrosshair)
		{
			ShowIntractableInfo(ActorUnderCrosshair,false);
		}
		ActorUnderCrosshair = nullptr;
	}
}

/*
 * Every Interactable Actor will have some info to show
 * This function toggle the info UI
*/
void UInteractionComponent::ShowIntractableInfo(AActor* IntractableActor, bool bShow)
{
	if(!IntractableActor) return;
	IIInteractable* InteractableActor = Cast<IIInteractable>(IntractableActor);
	if(InteractableActor)
	{
		InteractableActor->ShowInfo(bShow);
	
	}
}

// Player presses interact button
void UInteractionComponent::Interact()
{
	if(!ActorUnderCrosshair) return;
	// for now only using for picking up items
	// for now no inventory so player will equip the weapon
	AWeapon* WeaponToPick = Cast<AWeapon>(ActorUnderCrosshair);
	if(WeaponToPick)
	{
		FAttachmentTransformRules TransformRules{EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,true};
		WeaponToPick->Equip(Player->GetMesh(),TransformRules,FName("RightHandSocket"));
		WeaponToPick->SetOwner(Player);
		Player->SetCurrentWeapon(WeaponToPick);
		Player->SetCombatState(EPlayerCombatState::EPCS_SingleSword);
		Player->SelectedWeapon = WeaponToPick;
	}
}
