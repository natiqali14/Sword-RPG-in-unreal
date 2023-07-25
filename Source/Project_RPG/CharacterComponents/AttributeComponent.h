// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

USTRUCT(BlueprintType)
struct FAttribute
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float CurrentHealth;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float MaxHealth;
	
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_RPG_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Attributes,meta = (AllowPrivateAccess = true))
	FAttribute CharacterAttributes;
public:	
	// Sets default values for this component's properties
	UAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Getter & Setters
	FORCEINLINE FAttribute GetAttributes(){return CharacterAttributes;}
	

		
};
