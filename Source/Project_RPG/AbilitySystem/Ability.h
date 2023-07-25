// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityComponent.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "Ability.generated.h"

/** When a Ability Starts */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityActivation_,FName,AbilityName);
/** When a Ability stops */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityDeActivation,FName,AbilityName);

class UAbilityEffect;
USTRUCT(BlueprintType)
struct FAbilityProps
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName AbilityName; // Name of the Ability
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FString AbilityInfo; // Ability detail.
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool CoolDown = false; // Cool down time for Ability.
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float ManaCost; // Mana cost to use this ability.
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float DamageMultiplier = 1;
	
	
	
};
UCLASS(Blueprintable,BlueprintType)
class PROJECT_RPG_API UAbility : public UObject
{
	GENERATED_BODY()
protected:
	/** Game play tags that will be added to Ability Component on ability Activation. */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = GameplayTags,meta = (AllowPrivateAccess = true))
	FGameplayTagContainer OnActivatedTags;
	/** Gameplay tags that will block the ability activation. */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = GameplayTags,meta = (AllowPrivateAccess = true))
	FGameplayTagContainer BlockedTags;
	/** Props for Ability */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Props,meta = (AllowPrivateAccess = true))
	FAbilityProps AbilityProps;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Props,meta = (AllowPrivateAccess = true))
	bool bInCoolDown;
	/** Montage to play for this ability */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Anim | Montage",meta = (AllowPrivateAccess = true))
	UAnimMontage* AbilityMontage;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = GameplayTags,meta = (AllowPrivateAccess = true))
	FName AbilityName;
public:
	FAbilityActivation_ OnAbilityActivated;
	FAbilityDeActivation OnAbilityDeActivated;
	
	
public:
	UAbility();
	virtual void StartAbility(FGameplayTagContainer Blocked, AActor* PerformingActor, AController* Instigator);
	UFUNCTION(BlueprintCallable)
	virtual void StopAbility();
	virtual bool CanStartAbilities(FGameplayTagContainer Blocked);
	virtual  void InitiateAbilityMechanics(AActor* PerformingActor, AController* Instigator);
	virtual  void StopAbilityMechanics();
	virtual UWorld* GetWorld() const override;
	UAbilityComponent* GetOwningComponent();
	FORCEINLINE FGameplayTagContainer GetOnActivatedTags(){return OnActivatedTags;}
	FORCEINLINE FGameplayTagContainer GetBlockedTags(){return BlockedTags;}
	FORCEINLINE bool GetInCoolDown(){return bInCoolDown;}
	FORCEINLINE FAbilityProps GetAbilityProps(){return AbilityProps;}
	FORCEINLINE FName GetAbilityName(){return AbilityName;}
	
	
	
	
};
