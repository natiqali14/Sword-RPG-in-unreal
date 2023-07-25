// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "Components/BoxComponent.h"
#include "Weapon.generated.h"
USTRUCT(BlueprintType)
struct FWeaponInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FString WeaponName = "Old Sword";
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float WeaponDamage = 30;
	
};
/**
 * 
 */
UCLASS()
class PROJECT_RPG_API AWeapon : public AItem
{
	GENERATED_BODY()
	/** Box Component as a trigger for collisions with other objects while attacking */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Triggers, meta = (AllowPrivateAccess = true))
	UBoxComponent* WeaponTrigger;
	/** Trace Start point */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Tracing", meta = (AllowPrivateAccess = true))
	USceneComponent* TraceStart;
	/** Trace end point */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Tracing", meta = (AllowPrivateAccess = true))
	USceneComponent* TraceEnd;
	/** Trace Thickness */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Tracing", meta = (AllowPrivateAccess = true))
	FVector TraceBoxExtent;
	/** Triggering Actors */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Tracing", meta = (AllowPrivateAccess = true))
	TSet<AActor*> TriggeredActors;

	/** Weapon Information */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Weapon Info", meta = (AllowPrivateAccess = true))
	FWeaponInfo WeaponInfo;
	
public:
	AWeapon();
	virtual void BeginPlay() override;
	void Equip(USceneComponent* Parent,FAttachmentTransformRules Rules,FName SocketName);
	UFUNCTION(BlueprintCallable)
	void IsAttacking(bool bAttack); // Set collisions while attacking.

	// Getters and Setters
	FORCEINLINE USceneComponent* GetTraceStart(){return TraceStart;}
	FORCEINLINE USceneComponent* GetTraceEnd(){return TraceEnd;}
	FORCEINLINE FVector GetTraceBoxExtent(){return TraceBoxExtent;}
	FORCEINLINE FWeaponInfo GetWeaponInfo(){return WeaponInfo;}
	// Sword Trace
	void SwordTrace();
	// Weapon Triggers
	UFUNCTION()
	void WeaponTriggerBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void WeaponTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
		
	
};
