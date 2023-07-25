// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Project_RPG/MainCharacter/Aurora.h"
#include "Aurora_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RPG_API UAurora_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Props , meta = (AllowPrivateAccess = true))
	AAurora* AuroraRef;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Props , meta = (AllowPrivateAccess = true))
	float GroundSpeed;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Props , meta = (AllowPrivateAccess = true))
	bool bIsFalling;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Props , meta = (AllowPrivateAccess = true))
	float Direction;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Props , meta = (AllowPrivateAccess = true))
	float YawDelta;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Props , meta = (AllowPrivateAccess = true))
	float Turner;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Props , meta = (AllowPrivateAccess = true))
	ETurnInPlace TurnState;
	FRotator LastRotation;
	FRotator CurrentRotation;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = EditableProps , meta = (AllowPrivateAccess = true))
	float DeltaYawScaler;
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool bCombat;
	
	
	
};

// For Future Learning;
/*
* Thread Safe / Optimization of Anim instance Class (Optimization of Props for animations)
*/