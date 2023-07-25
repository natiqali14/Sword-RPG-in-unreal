// Fill out your copyright notice in the Description page of Project Settings.


#include "Aurora.h"


#include "Components/InteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Project_RPG/AbilitySystem/AbilityComponent.h"


AAurora::AAurora()
{
 	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	CamTurnSpeed  = 1.f;

	
	/** Movement Mechanics */
	MovementYawOffset = 0.f;

	/** Testing Vars */
	bCombatMode = false;

	/** Intraction Component */
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));

	/** Ability Component */
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("bility Component"));

	/** Player States */
	
	TurnInPlace = ETurnInPlace::ETIP_NotTurning;
	PlayerState_ = EPlayerState::EPS_UnOccupy;
	CombatState = EPlayerCombatState::EPCS_NonCombat;
}

void AAurora::BeginPlay()
{
	Super::BeginPlay();
	AbilityComponent->AbilityActivation.AddDynamic(this,&AAurora::OnAbilityActivation);
	
}
void AAurora::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//StateManager->PlayerToManage = this;
}

void AAurora::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CalculateYawOffset(DeltaTime);
//SimpleStrafeMechanics();

}

void AAurora::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	InteractionComponent->Player = this;
}

void AAurora::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind Axis
	/** For moving in X-Direction (Forward Movement) */
	PlayerInputComponent->BindAxis("MoveForward",this,&AAurora::MoveForward);
	/** For moving in Y-Direction (Side ways Movement) */
	PlayerInputComponent->BindAxis("MoveRight",this,&AAurora::MoveRight);
	/** For Looking up and down using mouse */
	PlayerInputComponent->BindAxis("Lookup",this,&AAurora::LookUp);
	/** For Turning camera / player with mouse (side ways) */
	PlayerInputComponent->BindAxis("Turn",this,&AAurora::Turn);
	
	
	// Bind Actions
	

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ACharacter::Jump);
	PlayerInputComponent->BindAction("Interact",IE_Pressed,this,&AAurora::InteractionButtonPressed);
	PlayerInputComponent->BindAction("LightAttack",IE_Pressed,this,&AAurora::LightAttackAbilityButtonPressed);
	PlayerInputComponent->BindAction("HeavyAttack",IE_Pressed,this,&AAurora::HeavyAttackButtonPressed);
	PlayerInputComponent->BindAction("Equip",IE_Pressed,this,&AAurora::EquipButtonPressed);
	
}

/*
 * Player basic movement
*/


void AAurora::MoveForward(float Value)
{
	if(Value == 0 || PlayerState_ == EPlayerState::EPS_Attacking) return;
	if( GetController())
	{
		FRotator ControlledRotation = GetController()->GetControlRotation();
		FRotator YawRotation = FRotator(0.f,ControlledRotation.Yaw,0.f);
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction,Value);
	}
}

void AAurora::MoveRight(float Value)
{
	if(Value == 0  || PlayerState_ == EPlayerState::EPS_Attacking) return;
	if( GetController())
	{
		FRotator ControlledRotation = GetController()->GetControlRotation();
		FRotator YawRotation = FRotator(0.f,ControlledRotation.Yaw,0.f);
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction,Value);
	}
}

void AAurora::Turn(float Value)
{
	if(Value == 0) return;
	
	
	AddControllerYawInput(Value * CamTurnSpeed);
	
}

void AAurora::LookUp(float Value)
{
	if(Value == 0) return;
	AddControllerPitchInput(Value * CamTurnSpeed);
	
}
// Movement Mechanics -> Yaw Offset Calculations
void AAurora::CalculateYawOffset(float DeltaTime)
{
	
	if(UKismetMathLibrary::VSizeXY(GetVelocity()) != 0 || GetMovementComponent()->IsFalling()
		|| PlayerState_ == EPlayerState::EPS_Attacking)
	{
		StartingAimBase = FRotator(0.f,GetBaseAimRotation().Yaw,0.f);
		MovementYawOffset = 0.f;
		TurnInPlace = ETurnInPlace::ETIP_NotTurning;
	}
	else
	{
		FRotator CurrentAimBase = FRotator(0.f,GetBaseAimRotation().Yaw,0.f);
		MovementYawOffset = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimBase,StartingAimBase).Yaw;
		if(TurnInPlace == ETurnInPlace::ETIP_NotTurning)
		{
			InterpingYawOffset = MovementYawOffset;
		}
		PlayerTurnInPlace(DeltaTime);
	}
}

void AAurora::PlayerTurnInPlace(float DeltaTime)
{
	if(MovementYawOffset > 90)
	{
		TurnInPlace = ETurnInPlace::ETIP_TurnInRight;
	}
	if(MovementYawOffset < - 90)
	{
		TurnInPlace = ETurnInPlace::ETIP_TurnInLeft;
	}
	if(TurnInPlace != ETurnInPlace::ETIP_NotTurning)
	{
		InterpingYawOffset = FMath::FInterpTo(InterpingYawOffset, 0.f, DeltaTime, 4.f);
		MovementYawOffset = InterpingYawOffset;
		if (FMath::Abs(MovementYawOffset) < 15.f)
		{
			TurnInPlace = ETurnInPlace::ETIP_NotTurning;
			StartingAimBase = FRotator(0.f, GetBaseAimRotation().Yaw - MovementYawOffset, 0.f);
		}
	}
}

void AAurora::SimpleStrafeMechanics() // Legacy.
{
	if(CombatState == EPlayerCombatState::EPCS_NonCombat) return;
	if(UKismetMathLibrary::VSizeXY(GetVelocity()) != 0 || GetMovementComponent()->IsFalling() || PlayerState_ == EPlayerState::EPS_Attacking )
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

// Combat Mechanics
void AAurora::SetCombatMode(bool bInCombat)
{
	bCombatMode = bInCombat;
	if(bCombatMode)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}


/**
* Items related
*/
void AAurora::AddOverlappingItems(AItem* Item)
{
	if(!Item) return;
	OverlappingItems.Add(Item);
	if(InteractionComponent && !InteractionComponent->GetCanRayCast())
	{
		InteractionComponent->SetCanRayCast(true);
	}
}

void AAurora::RemoveOverlappingItems(AItem* Item)
{
	if(!Item) return;
	OverlappingItems.Remove(Item);
	Item->ShowInfo(false);
	if(OverlappingItems.Num() < 1 && InteractionComponent && InteractionComponent->GetCanRayCast())
	{
		InteractionComponent->SetCanRayCast(false);
		
	}
}


/**
* Interactions
*/

void AAurora::InteractionButtonPressed()
{
	InteractionComponent->Interact();
	
	
}
/**
 *Abilities
 */
void AAurora::LightAttackAbilityButtonPressed()
{
	if(!WeaponEquipped || PlayerState_ != EPlayerState::EPS_UnOccupy) return;
	
	AbilityComponent->AbilityCaller(WeaponEquipped,GetController(),0);
	
}

void AAurora::HeavyAttackButtonPressed()
{
	if(!WeaponEquipped||PlayerState_ != EPlayerState::EPS_UnOccupy)  return;
	AbilityComponent->AbilityCaller(WeaponEquipped,GetController(),1);
}


UAbility* AAurora::GetCurrentAbility()
{
	return AbilityComponent->GetCurrentAbility();
}


// Player States
void AAurora::SetPlayerState(EPlayerState State)
{
	PlayerState_ = State;
}




void AAurora::SetCombatState(EPlayerCombatState State)
{
	CombatState = State;
	if(State == EPlayerCombatState::EPCS_SingleSword)
	{
		SetCombatMode(true);
	}
	else if(State == EPlayerCombatState::EPCS_NonCombat)
	{
		SetCombatMode(false);
	}
}

// Ability Component
void AAurora::OnAbilityActivation(bool bActivation)
{
	if(bActivation)
	{
		SetPlayerState(EPlayerState::EPS_Attacking);
	}
	else
	{
		SetPlayerState(EPlayerState::EPS_UnOccupy);
	}
}

void AAurora::InitiateCurrentAbilityMechanics()
{
	AbilityComponent->CurrentAbilityMechanicsInit(WeaponEquipped,GetController());	
}

// Montages
// Equipping Montage
void AAurora::PlayEquipMontage(bool bEquip)
{
	if(!EquipMontage) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(!AnimInstance) return;
	FName EquipName;
	if(bEquip)
	{
		 EquipName = FName("Equip");
	}
	else
	{
		 EquipName = FName("UnEquip");
	}
	 
	AnimInstance->Montage_Play(EquipMontage);
	AnimInstance->Montage_JumpToSection(EquipName,EquipMontage);
}

void AAurora::EquipButtonPressed()
{
	if(PlayerState_ != EPlayerState::EPS_UnOccupy) return;
	if(CombatState == EPlayerCombatState::EPCS_NonCombat && SelectedWeapon)
	{
		PlayEquipMontage(true);
	}
	else
	{
		if(!WeaponEquipped) return;
		PlayEquipMontage(false);
	}
}
// Weapon code
void AAurora::SetCurrentWeapon(AWeapon* Weapon)
{
	if(Weapon)
	{
		WeaponEquipped = Weapon;
		SetCombatState(EPlayerCombatState::EPCS_SingleSword);
		
	}
	else
	{
		WeaponEquipped = nullptr;
		SetCombatState(EPlayerCombatState::EPCS_NonCombat);
	}
}
void AAurora::EquipSword()
{
	if(!SelectedWeapon) return;
	FAttachmentTransformRules TransformRules{EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,true};
	SelectedWeapon->Equip(GetMesh(),TransformRules,FName("RightHandSocket"));
	SetCurrentWeapon(SelectedWeapon);
}

void AAurora::UnEquipSword()
{
	if(!WeaponEquipped) return;
	FAttachmentTransformRules TransformRules{EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,true};
	WeaponEquipped->Equip(GetMesh(),TransformRules,FName("SpineSocket"));
	SetCurrentWeapon(nullptr);
	
}
