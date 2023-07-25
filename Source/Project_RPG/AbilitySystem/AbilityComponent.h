

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityActivation,bool,bActivated);

USTRUCT(BlueprintType)
struct FAbilityTuple
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FName AbilityName;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<class UAbility> Ability;
	
};

USTRUCT(BlueprintType)
struct FActorArray
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<AActor*> Actors;
	
};
class UAbility;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_RPG_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()
	/** Total Abilities that are Acquired by Player. */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Abilities", meta = (AllowPrivateAccess = true))
	TSet<UAbility*> AcquiredAbilities;
	/** Abilities that are selected to use at a specific time by player
	 * As for know there will be 5 selected abilities first 2 will be starter abilities, other 3 wil be special. 
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Abilities", meta = (AllowPrivateAccess = true))
	TArray<UAbility*> SelectedAbilities;
	/** Light Attack ability name */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Abilities", meta = (AllowPrivateAccess = true))
	FName LightAttackAbility;
	/** Heavy Attack ability name */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Abilities", meta = (AllowPrivateAccess = true))
	FName HeavyAttackAbility;
	
	/** Gameplay tags acquired */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Abilities | Tags", meta = (AllowPrivateAccess = true))
	FGameplayTagContainer GrantedTags;

	/** Current Active Ability */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Abilities | Current", meta = (AllowPrivateAccess = true))
	UAbility* CurrentAbility;
	/** Player Abilities Stack */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Abilities | Stack", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UAbility>> AbilityStack;
	/** Map that have info of actors effected by ability (only 1 pass). After ability finished the slot will
	 * be initialised
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Abilities", meta = (AllowPrivateAccess = true))
	TMap<UAbility*, FActorArray> HitActors;

	
	
	
	
	
public:
	FAbilityActivation AbilityActivation;

	
	// Sets default values for this component's properties
	UAbilityComponent();

	UAbility* AcquireAbility(TSubclassOf<UAbility> AbilityToAcquire);
	void LightAttackAbilityCalled(AActor* Actor , AController* Instigator);
	void HeavyAttackAbilityCalled(AActor* Actor , AController* Instigator);
	void AbilityCaller(AActor* Actor , AController* Instigator, uint8_t AbilityIndex);
	UFUNCTION()
	void OnAbilityDeActivated(FName Ability);
	UFUNCTION()
	void OnAbilityActivated(FName Ability);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void CurrentAbilityMechanicsInit(AActor* PerformingActor, AController* Instigator);
	void StopCurrentAbilityMechanics();
	void InitializeStarterAbilities();
	void SetHitActors(UAbility* Ability, FActorArray ActorArray);
	void InitializeHitActors();
	void AddHitActor(UAbility* Ability, AActor* Actor);
	bool HitActorFound(UAbility* Ability, AActor* Actor);
	void ResetHitActor(UAbility* Ability);

	FORCEINLINE void RemoveGrantedTags(FGameplayTagContainer Tags){GrantedTags.RemoveTags(Tags);}

	FORCEINLINE FGameplayTagContainer GetGrantedTags(){return GrantedTags;}
	FORCEINLINE void SetGrantedTags(FGameplayTagContainer Tags){GrantedTags.AppendTags(Tags);}
	FORCEINLINE UAbility* GetCurrentAbility(){return CurrentAbility;}
	FORCEINLINE void SetCurrentAbility(UAbility* Ability){CurrentAbility = Ability;}
	FORCEINLINE TMap<UAbility*, FActorArray> GetHitActors(){return HitActors;}
	

	
	UFUNCTION(BlueprintCallable)
	void AddToSelectedAbility(UAbility* Ability, uint8 Index);
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void RemoveFromSelectedAbility(UAbility* Ability){if(Ability)SelectedAbilities.Remove(Ability);}

	
};

