


#include "AttributeComponent.h"


UAttributeComponent::UAttributeComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	CharacterAttributes.MaxHealth = 100;
	

	
}



void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterAttributes.CurrentHealth = CharacterAttributes.MaxHealth;


	
}



void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

