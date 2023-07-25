// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Project_RPG/Enemy/BaseEnemy.h"
#include "BaseEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RPG_API UBaseEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess))
	ABaseEnemy* BaseEnemy;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess))
	EDeathPose DeathState;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess))
	float Speed;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess = true))
	float StrafeBlendSpaceAnimSpeed;
	
};
