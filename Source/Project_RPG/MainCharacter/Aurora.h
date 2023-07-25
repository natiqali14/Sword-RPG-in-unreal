// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../../UE_5.0.2/UE_5.0/Engine/Source/Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "../../../../UE_5.0.2/UE_5.0/Engine/Source/Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Components/PlayerStateManager.h"
#include "GameFramework/Character.h"
#include "Project_RPG/AbilitySystem/Ability.h"
#include "Project_RPG/Items/Item.h"
#include "Project_RPG/Items/Weapons/Weapon.h"
#include "Aurora.generated.h"

UCLASS()
class PROJECT_RPG_API AAurora : public ACharacter
{
	GENERATED_BODY()
private:
	/** Camera for Aurora */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Camera, meta = (AllowPrivateAccess = true))
	UCameraComponent* FollowCamera;
	/** Spring Arm for Camera */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Camera, meta = (AllowPrivateAccess = true))
	USpringArmComponent* CameraBoom;
	/** Turning Speed for Player's Camera */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Camera | Props",meta = (AllowPrivateAccess = true))
	float CamTurnSpeed;
	/** State Manager for Player's different states */
	// UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "State Manager",meta = (AllowPrivateAccess = true))
	// UPlayerStateManager* StateManager;
	/** Players Aimbase Rotation -> Updating every frame iff player is'nt idle */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Movement Mechanics|Yaw Offsets",meta = (AllowPrivateAccess = true))
	FRotator StartingAimBase;
	float MovementYawOffset;
	float InterpingYawOffset;

	/** Overlapping Items Array */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Items",meta = (AllowPrivateAccess = true))
	TSet<AItem*> OverlappingItems;

	/** Interaction Component */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Interaction",meta = (AllowPrivateAccess = true))
	class UInteractionComponent* InteractionComponent;

	/** Ability Component */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Ability Component",meta = (AllowPrivateAccess = true))
	class UAbilityComponent* AbilityComponent;

	/** Current Weapon Equipped by the player */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Ability Component",meta = (AllowPrivateAccess = true))
	class AWeapon* WeaponEquipped;

	/** Player turning state */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = TurnInPlace,meta = (AllowPrivateAccess = true))
	ETurnInPlace TurnInPlace;
	/** Player Combat State */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = CombatState,meta = (AllowPrivateAccess = true))
	EPlayerCombatState CombatState;

	/** Player State */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = PlayerState,meta = (AllowPrivateAccess = true))
	EPlayerState PlayerState_;
	
	/** Un/Equipping Montage */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Montages,meta = (AllowPrivateAccess = true))
	UAnimMontage* EquipMontage;
	
	
public:
	
	UPROPERTY()
	AWeapon* SelectedWeapon; // Only for test purpose will be connected with UI;

	
	AAurora();
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool bCombatMode; // For Testing only;
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void PreInitializeComponents() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	FORCEINLINE float GetMovementOffsetYaw(){return MovementYawOffset;}
	FORCEINLINE ETurnInPlace GetTurnInPlace(){return TurnInPlace;}

	// Items related functions
	void AddOverlappingItems(AItem* Item); // Add a Overlapping item to the OverlappingItems Array
	void RemoveOverlappingItems(AItem* Item); // Removing a Overlapping item to the OverlappingItems Array
	FORCEINLINE TSet<AItem*> GetOverlappingItems(){return OverlappingItems;}
	
private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	// Movement mechanics
	void CalculateYawOffset(float DeltaTime); 
	void PlayerTurnInPlace(float DeltaTime);
	void SimpleStrafeMechanics();
	

	// Interaction Component

	void InteractionButtonPressed();

	// Abilities
	void LightAttackAbilityButtonPressed();
	void HeavyAttackButtonPressed();
	UFUNCTION(BlueprintCallable)
	UAbility* GetCurrentAbility();

	/**
	 *Un/Equipping Montage
	 */
	void PlayEquipMontage(bool bEquip);
	void EquipButtonPressed();
public:
	// Combat Mechanics
	void SetCombatMode(bool bInCombat);

	// Weapons
	UFUNCTION(BlueprintCallable)
	FORCEINLINE AWeapon* GetCurrentWeapon(){return WeaponEquipped;}
	void SetCurrentWeapon(AWeapon* Weapon);

	// Combat State
	UFUNCTION(BlueprintCallable)
	void SetCombatState(EPlayerCombatState State);
	FORCEINLINE EPlayerCombatState GetCombatState(){return CombatState;}

	// Player State
	UFUNCTION(BlueprintCallable)
	void SetPlayerState(EPlayerState State);
	FORCEINLINE EPlayerState GetPlayerState(){return PlayerState_;}

	// Ability Component
	UFUNCTION()
	void OnAbilityActivation(bool bActivation);
	UFUNCTION(BlueprintCallable)
	void InitiateCurrentAbilityMechanics();
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void StopCurrentAbilityMechanics(){AbilityComponent->StopCurrentAbilityMechanics();}

	// Weapon Code
	UFUNCTION(BlueprintCallable)
	void EquipSword();
	UFUNCTION(BlueprintCallable)
	void UnEquipSword();
};


