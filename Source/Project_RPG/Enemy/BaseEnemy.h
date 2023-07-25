// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitInterface.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Project_RPG/AI/Controllers/BaseEnemyAIController.h"
#include "Project_RPG/Enums/CustomStates.h"
#include "Sound/SoundCue.h"
#include "BaseEnemy.generated.h"
USTRUCT(BlueprintType)
struct FEnemyInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FName EnemyName = FName("Grux");
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FString EnemyLevel = "Lv.1";
	
};
UCLASS()
class PROJECT_RPG_API ABaseEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()
	/** Montage on hit */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montages | Hit",meta = (AllowPrivateAccess = true))
	UAnimMontage* HitMontage;
	/** Montage on Death */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montages | Death",meta = (AllowPrivateAccess = true))
	UAnimMontage* DeathMontage;
	/** Sound, When enemy got hit */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Sounds | Pain", meta = (AllowPrivateAccess = true))
	USoundCue* PainSound;
	/** Sound, When enemy Dies */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Sounds | Death", meta = (AllowPrivateAccess = true))
	USoundCue* DeathSound;
	/** Particle, When enemy got hit */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "VFX | Bleed", meta = (AllowPrivateAccess = true))
	UParticleSystem* BleedParticle;
	/** Current enemy Health */ 
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Health", meta = (AllowPrivateAccess = true))
	float CurrentHealth;
	/** Current enemy Max Health */ 
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Health", meta = (AllowPrivateAccess = true))
	float MaxHealth;
	/** Health Bar widget got enemy */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "UI |Health", meta = (AllowPrivateAccess = true))
	UWidgetComponent* HealthBarComponent;
	/** Basic Enemy info */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Info", meta = (AllowPrivateAccess = true))
	FEnemyInfo EnemyInfo; // Will use it for making different enemies with Data table
	UPROPERTY()
	float LastHitAngle; // Latest Hit point on enemy.
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "States | Death", meta = (AllowPrivateAccess = true))
	EDeathPose EnemyDeathState;
	/** Time after which Enemy Actor will be destroyed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Enemy Death",meta = (AllowPrivateAccess = true))
	float DeathTime;
	FTimerHandle DeathTimerHandle;
	/** Time to show health bar  */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "UI |Health", meta = (AllowPrivateAccess = true))
	float HeathBarVisibilityTime;
	FTimerHandle HealthVisibilityHandle;

	// Ai
	/** BT for Base Enemy */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "AI | BT", meta = (AllowPrivateAccess = true))
	UBehaviorTree* BehaviorTree;
	/** Enemy Patrol points*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "AI | Patrol Points", meta = (AllowPrivateAccess = true))
	TArray<AActor*> PatrolPoints;
	/** Enemy State */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "AI | State", meta = (AllowPrivateAccess = true))
	EEnemyState EnemyState;
	/** Enemy Aggro Sphere */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "AI | Agro", meta = (AllowPrivateAccess = true))
	USphereComponent* AgroSphere;
	/** Next patrol location for enemy */
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "AI | Patrol Points", meta = (AllowPrivateAccess = true))
	AActor* NextPatrolPoint;
	/** Pawn Sensing component */
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "AI | Pawn Sensing", meta = (AllowPrivateAccess = true))
	UPawnSensingComponent* PawnSensingComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "AI | Pawn Sensing", meta = (AllowPrivateAccess = true))
	UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "AI | Pawn Sensing", meta = (AllowPrivateAccess = true))
	APawn* TargetToChase;
	/** After losing Target */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI | Pawn Sensing", meta = (AllowPrivateAccess = true))
	float LookAroundTime;
	FTimerHandle PatrolHandle;
	FTimerHandle LookAroundStateHandle;
	
	/** Combat sphere */
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "AI | Pawn Sensing", meta = (AllowPrivateAccess = true))
	USphereComponent* AttackingRadius;
	/** Location where enemy spawned */
	UPROPERTY(VisibleAnywhere)
	FVector SpawnPoint;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true) )
	float EnemyRange;
	

	
	UPROPERTY(VisibleAnywhere)
	ABaseEnemyAIController* EnemyAIController;


	/*Only cpp vars */
	int LastIndex = -1; // last patrol point index for enemy
	int NewIndex = -1;  // current patrol point index for enemy
	UPROPERTY()
	AActor* LastPatrolPoint;
	
		

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void PlayHitMontage(double Theta); // Montage when player got hit
	virtual void GetHit(FVector ImpactPoint) override; // Hit functionality.
	double GetHitTheta( FVector HitImpactPoint); // Calculating angle of hit point.
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
		AActor* DamageCauser) override; // Damage taken by Enemy

	void Die(); // Enemy Death.
	void PlayDeathMontage(); // Death montage
	void PlayWidgetAnimationsOnDeath();

	void EnemyDestroy();

	// UI | Health Bar
	void HideHealthBar();

	// Enemy State
	void SetEnemyState(EEnemyState State);
	void EnemyChasing();

	/*
	 *AI
	 */
	// Patrolling
	UFUNCTION(BlueprintCallable)
	AActor* GetNextPatrolPoint();

	// Chasing / Sensing
	UFUNCTION()
	void OnSeeTarget(APawn* Pawn);
	void GoBackToPatrolling();
	void SetTargetToChase(APawn* Target);
	
	// Look Around
	void LookAround();
	UFUNCTION()
	void OnChasingRadiusEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Timers handling
	void ClearAllTimers();
	// Getters & Setters
	FORCEINLINE FEnemyInfo GetEnemyInfo(){return EnemyInfo;}
	FORCEINLINE EDeathPose GetEnemyDeathState(){return EnemyDeathState;}
	FORCEINLINE UBehaviorTree* GetBehaviorTree(){return BehaviorTree;}
	FORCEINLINE TArray<AActor*> GetPatrolPoints(){return PatrolPoints;}
	FORCEINLINE EEnemyState GetEnemyState(){return EnemyState;}
private:
	// Utility functions
	EDeathPose CalculateDeathPose(int Index);
};





