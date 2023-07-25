// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInteractable.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class PROJECT_RPG_API AItem : public AActor, public IIInteractable
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Mesh,meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* ItemMesh;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Triggers",meta = (AllowPrivateAccess = true))
	USphereComponent* SphereCollider;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Triggers",meta = (AllowPrivateAccess = true))
	UWidgetComponent* ItemInfo;
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UStaticMeshComponent* GetItemMesh(){return ItemMesh;}
	FORCEINLINE USphereComponent* GetSphereCollider(){return SphereCollider;}

	 //void ShowInfo(bool bShow) ;
	virtual void ShowInfo(bool bShow) override;
private:
	// OnBeginComponent Delegate for sphere collider
	UFUNCTION()
	void OnBeginSphereCollider(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnEndSphereCollider(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
