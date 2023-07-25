// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BaseEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RPG_API ABaseEnemyAIController : public AAIController
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere)
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditAnywhere)
	UBehaviorTreeComponent* BehaviorTreeComponent;
public:
	ABaseEnemyAIController();
	virtual void OnPossess(APawn* InPawn) override;
	void SetKeyValueAsAString(FName KeyName, FName KeyValue);
	void SetValueAsAActor(FName KeyName, AActor* KeyValue);
	void SetNextPatrolPoint(AActor* Point);
	UFUNCTION(BlueprintCallable)
	void SetIsPatroling(bool bPatroling);
	UFUNCTION(BlueprintCallable)
	void SetIsAttacking(bool bAttacking);
	UFUNCTION(BlueprintCallable)
	void SetTargetToChase(AActor* Target);
	UFUNCTION(BlueprintCallable)
	void SetIsLooking(bool bLooking);
	UFUNCTION(BlueprintCallable)
	void SetIsChasing(bool bChase);
	
	
};
