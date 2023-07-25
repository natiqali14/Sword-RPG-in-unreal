// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"

#include "Engine/ActorChannel.h"

UAbility::UAbility()
{
}

void UAbility::StartAbility(FGameplayTagContainer Blocked,AActor* PerformingActor, AController* Instigator)
{
	
	if(!CanStartAbilities(Blocked)) return;
	
	GetOwningComponent()->SetCurrentAbility(this);
	OnAbilityActivated.Broadcast(AbilityProps.AbilityName);
}

void UAbility::StopAbility()
{
	GetOwningComponent()->RemoveGrantedTags(OnActivatedTags);
	GetOwningComponent()->SetCurrentAbility(nullptr);
	OnAbilityDeActivated.Broadcast(AbilityProps.AbilityName);
}

bool UAbility::CanStartAbilities(FGameplayTagContainer Blocked)
{
	if(bInCoolDown) return false;
	if(Blocked.HasAny(Blocked)) return false;
	return true;
}

void UAbility::InitiateAbilityMechanics(AActor* PerformingActor, AController* Instigator)
{
	// Actual implementation in the child class
}

void UAbility::StopAbilityMechanics()
{
	
}

UWorld* UAbility::GetWorld() const
{
	UActorComponent* ActorComponent = Cast<UActorComponent>(GetOuter());
	if(ActorComponent) return ActorComponent->GetWorld();
	return nullptr;
}

UAbilityComponent* UAbility::GetOwningComponent()
{
	return Cast<UAbilityComponent>(GetOuter());
}
