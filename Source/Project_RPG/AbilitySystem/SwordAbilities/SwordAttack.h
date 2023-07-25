// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ability.h"
#include "SwordAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RPG_API USwordAttack : public UAbility
{
	GENERATED_BODY()
	/** Montage Section names */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Montage, meta = (AllowPrivateAccess = true))
	TArray<FName>MontageSectionName;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Montage, meta = (AllowPrivateAccess = true))
	int32 LastAttack;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Montage, meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> DamageType;
	FTimerHandle SwordTraceHandle;
	
public:
	USwordAttack();
	virtual void StartAbility(FGameplayTagContainer Blocked,AActor* PerformingActor, AController* Instigator) override;
	virtual void StopAbility() override;
	virtual bool CanStartAbilities(FGameplayTagContainer Blocked) override;
	virtual  void InitiateAbilityMechanics(AActor* PerformingActor, AController* Instigator) override;
	virtual void StopAbilityMechanics() override;

	void PlayAbilityMontage(UAnimInstance* AnimInstance);
	TArray<FHitResult> SwordTrace(class AWeapon* SwordWeapon);
	UFUNCTION()
	void SwordTraceLoop(AActor* PerformingActor, AController* Instigator);
	void SwordTraceCaller(AActor* PerformingActor, AController* Instigator);
	// Get the angle for Enemy Hit Reaction montage;
	
	
};






