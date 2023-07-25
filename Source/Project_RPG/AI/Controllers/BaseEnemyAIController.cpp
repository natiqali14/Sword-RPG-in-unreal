// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Project_RPG/Enemy/BaseEnemy.h"

ABaseEnemyAIController::ABaseEnemyAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BT Comp");
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("BB Comp");
}

void ABaseEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(InPawn)
	{
		ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(InPawn);
		if(!BaseEnemy) return;
		if(BaseEnemy->GetBehaviorTree())
		{
			BlackboardComponent->InitializeBlackboard(*(BaseEnemy->GetBehaviorTree()->BlackboardAsset));
		}
	}
	
	
}

void ABaseEnemyAIController::SetKeyValueAsAString(FName KeyName, FName KeyValue)
{
}

void ABaseEnemyAIController::SetValueAsAActor(FName KeyName, AActor* KeyValue)
{
	BlackboardComponent->SetValueAsObject(KeyName,KeyValue);
}

void ABaseEnemyAIController::SetNextPatrolPoint(AActor* Point)
{
	BlackboardComponent->SetValueAsObject(FName("NextPatrolPoint"),Point);
}

void ABaseEnemyAIController::SetIsPatroling(bool bPatroling)
{
	BlackboardComponent->SetValueAsBool(FName("IsPatroling"), bPatroling);
	BlackboardComponent->SetValueAsBool(FName("IsAttacking"),false);;
	BlackboardComponent->SetValueAsBool(FName("IsLooking"), false);
	BlackboardComponent->SetValueAsBool(FName("IsChasing"),false);

	
}

void ABaseEnemyAIController::SetIsAttacking(bool bAttacking)
{
	BlackboardComponent->SetValueAsBool(FName("IsAttacking"),bAttacking);;
	BlackboardComponent->SetValueAsBool(FName("IsLooking"), false);
	BlackboardComponent->SetValueAsBool(FName("IsPatroling"), false);
	BlackboardComponent->SetValueAsBool(FName("IsChasing"),false);
	
}

void ABaseEnemyAIController::SetTargetToChase(AActor* Target)
{
	BlackboardComponent->SetValueAsObject(FName("TargetToChase"),Target);
}

void ABaseEnemyAIController::SetIsLooking(bool bLooking)
{
	BlackboardComponent->SetValueAsBool(FName("IsLooking"), bLooking);
	BlackboardComponent->SetValueAsBool(FName("IsAttacking"),false);	
	BlackboardComponent->SetValueAsBool(FName("IsPatroling"), false);
	BlackboardComponent->SetValueAsBool(FName("IsChasing"),false);
	
}

void ABaseEnemyAIController::SetIsChasing(bool bChase)
{
	BlackboardComponent->SetValueAsBool(FName("IsChasing"),bChase);
	BlackboardComponent->SetValueAsBool(FName("IsLooking"), false);
	BlackboardComponent->SetValueAsBool(FName("IsAttacking"),false);	
	BlackboardComponent->SetValueAsBool(FName("IsPatroling"), false);
}
