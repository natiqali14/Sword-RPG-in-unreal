// Fill out your copyright notice in the Description page of Project Settings.


#include "Aurora_AnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/PawnMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"

void UAurora_AnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	AuroraRef = Cast<AAurora>(TryGetPawnOwner());
	GroundSpeed = 0;
	bIsFalling = false;
	LastRotation = FRotator::ZeroRotator;
	CurrentRotation = FRotator::ZeroRotator;
	YawDelta = 0;
	DeltaYawScaler = 7;
	Turner = 0;
	bCombat = false;
	
}


void UAurora_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(!AuroraRef)
	{
		AuroraRef = Cast<AAurora>(TryGetPawnOwner());
	}
	if(!AuroraRef) return;
	GroundSpeed = UKismetMathLibrary::VSizeXY(AuroraRef->GetVelocity());
	bIsFalling = AuroraRef->GetMovementComponent()->IsFalling();
	FRotator Dir = UKismetMathLibrary::MakeRotFromX(AuroraRef->GetVelocity());
	Direction = UKismetMathLibrary::NormalizedDeltaRotator(Dir,AuroraRef->GetBaseAimRotation()).Yaw;
	CurrentRotation = AuroraRef->GetActorRotation();
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(CurrentRotation,LastRotation);
	float Delta = DeltaRot.Yaw;
	
	Delta /= DeltaSeconds;
	//Delta /= DeltaYawScaler;
	float Interp = FMath::FInterpTo(YawDelta,Delta,DeltaSeconds,DeltaYawScaler);
	YawDelta = FMath::Clamp(Interp,-45,45);
	LastRotation = CurrentRotation;
	Turner  = AuroraRef->GetMovementOffsetYaw();
	TurnState = AuroraRef->GetTurnInPlace();
	bCombat = AuroraRef->bCombatMode; // Getting bCombat from test Var;
	
}
