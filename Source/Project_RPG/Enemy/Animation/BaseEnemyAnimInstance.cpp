// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAnimInstance.h"

#include "Kismet/KismetMathLibrary.h"

void UBaseEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	BaseEnemy = Cast<ABaseEnemy>(TryGetPawnOwner());
	DeathState = EDeathPose::EDP_Alive;
	Speed = 0;
	StrafeBlendSpaceAnimSpeed = 1.f;
}

void UBaseEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(!BaseEnemy)
	{
		BaseEnemy = Cast<ABaseEnemy>(TryGetPawnOwner());
	}
	if(BaseEnemy)
	{
		DeathState = BaseEnemy->GetEnemyDeathState();
		Speed = UKismetMathLibrary::VSizeXY(BaseEnemy->GetVelocity());
	}
	
}
