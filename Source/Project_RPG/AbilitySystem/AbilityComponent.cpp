


#include "AbilityComponent.h"

#include "Ability.h"
#include "GameFramework/Character.h"



UAbilityComponent::UAbilityComponent()
{	
	PrimaryComponentTick.bCanEverTick = true;	
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	SelectedAbilities.Init(nullptr,5);
	InitializeStarterAbilities();
}

void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);	
}

void UAbilityComponent::InitializeStarterAbilities()
{
	for(int i = 0; i <2; i++)
	{
		if(AbilityStack[i])
		{
			SelectedAbilities[i] = AcquireAbility(AbilityStack[i]);
		}
	}
}

void UAbilityComponent::SetHitActors(UAbility* Ability, FActorArray ActorArray)
{
	if(!Ability) return;
	HitActors[Ability] = ActorArray;
}

void UAbilityComponent::InitializeHitActors()
{
	HitActors = TMap<UAbility*,FActorArray>();
}

void UAbilityComponent::AddHitActor(UAbility* Ability, AActor* Actor)
{
	if(HitActors.Contains(Ability))
	{
		HitActors[Ability].Actors.AddUnique(Actor);
		return;
	}
	FActorArray NewArray;
	NewArray.Actors.AddUnique(Actor);
	HitActors.Add(Ability,NewArray);
}

bool UAbilityComponent::HitActorFound(UAbility* Ability, AActor* Actor)
{
	if(HitActors.Contains(Ability) && HitActors[Ability].Actors.Contains(Actor))
	{
		return true;
	}
	return false;
}

void UAbilityComponent::ResetHitActor(UAbility* Ability)
{
	if(HitActors.Contains(Ability))
	{
		HitActors[Ability].Actors.Empty();
	}
}

void UAbilityComponent::AddToSelectedAbility(UAbility* Ability, uint8_t Index)
{
	if(Ability && Index <= SelectedAbilities.Num())
	{
		SelectedAbilities[Index] = Ability;
	}
}

void UAbilityComponent::CurrentAbilityMechanicsInit(AActor* PerformingActor, AController* Instigator)
{
	if(CurrentAbility)
	{
		CurrentAbility->InitiateAbilityMechanics(PerformingActor,Instigator);
	}
}

void UAbilityComponent::StopCurrentAbilityMechanics()
{
	if(CurrentAbility)
	{
		CurrentAbility->StopAbilityMechanics();
	}
}


/** When player gets a new ability */
UAbility* UAbilityComponent::AcquireAbility(TSubclassOf<UAbility> AbilityToAcquire)
{
	if(!AbilityToAcquire) return nullptr;
	UAbility* AcquiredAbility = NewObject<UAbility>(this,AbilityToAcquire);
	
	if(AcquiredAbility)
	{
		AcquiredAbilities.Add(AcquiredAbility);
	
		// for now adding to selected abilities automatically,
		
		AcquiredAbility->OnAbilityActivated.AddDynamic(this,&UAbilityComponent::OnAbilityActivated);
		AcquiredAbility->OnAbilityDeActivated.AddDynamic(this,&UAbilityComponent::OnAbilityDeActivated);
		
	}
	return AcquiredAbility;
	
	
}
void UAbilityComponent::AbilityCaller(AActor* Actor, AController* Instigator, uint8_t AbilityIndex)
{
	if(!Actor || !Instigator) return;
	
	if(SelectedAbilities.Num() > AbilityIndex && SelectedAbilities[AbilityIndex])
	{
		
		SelectedAbilities[AbilityIndex]->StartAbility(GrantedTags,Actor, Instigator);
	}
}

void UAbilityComponent::LightAttackAbilityCalled(AActor* Actor, AController* Instigator)
{
	
	
}

// void UAbilityComponent::HeavyAttackAbilityCalled(AActor* Actor , AController* Instigator)
// {
// 	if(!Actor || !Instigator) return;
// 	
// 	if( SelectedAbilities.Contains(HeavyAttackAbility))
// 	{
// 		SelectedAbilities[HeavyAttackAbility]->StartAbility(GrantedTags,Actor, Instigator);
// 	}
// }

// function for Ability DeActivation Ability
void UAbilityComponent::OnAbilityDeActivated(FName Ability)
{
	AbilityActivation.Broadcast(false);	
}

void UAbilityComponent::OnAbilityActivated(FName Ability)
{
	AbilityActivation.Broadcast(true);
	UE_LOG(LogTemp,Error,TEXT("OnAbilityActivated"))
}

