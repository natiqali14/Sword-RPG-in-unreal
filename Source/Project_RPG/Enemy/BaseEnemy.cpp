


#include "BaseEnemy.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Project_RPG/MainCharacter/Aurora.h"
#include "Project_RPG/UI/HealthBar.h"


ABaseEnemy::ABaseEnemy()
{
 	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	MaxHealth = 100;

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar Component"));
	HealthBarComponent->SetupAttachment(GetRootComponent());
	EnemyDeathState = EDeathPose::EDP_Alive;
	DeathTime = 1;
	HeathBarVisibilityTime = 3.f;

	//AI
	EnemyState = EEnemyState::EES_Patrolling;
	EnemyRange = 1600.f;
	// Pawn Sensing
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Sensing");
	//AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("Ai Perception");
	AttackingRadius = CreateDefaultSubobject<USphereComponent>("Attacking Radius");
	AttackingRadius->SetupAttachment(GetRootComponent());
	LookAroundTime = 3.f;
	LastPatrolPoint = nullptr;

	
	// Agro
	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Agro Sphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	NextPatrolPoint = nullptr;

	// Ai Looking
	EnemyRange = 600.f;
}


void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	if(UHealthBar* HealthBar = Cast<UHealthBar>(HealthBarComponent->GetWidget()))
	{
		HealthBar->SetLevel(EnemyInfo.EnemyLevel);
	}
	EnemyAIController = Cast<ABaseEnemyAIController>(GetController());
	SetEnemyState(EEnemyState::EES_Patrolling);
	PawnSensingComponent->OnSeePawn.AddDynamic(this,&ABaseEnemy::OnSeeTarget);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this,&ABaseEnemy::OnChasingRadiusEnd);
	SpawnPoint = GetActorLocation();
	EnemyAIController->SetIsPatroling(true);
	//EDetachmentRule DetachmentRule{EDetachmentRule::KeepWorld};
	const FDetachmentTransformRules Rules{EDetachmentRule::KeepWorld,true};
	AgroSphere->DetachFromComponent(Rules);
	
	
	
}


void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector CameraLoc;
	FRotator CameraRot;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(CameraLoc,CameraRot);
	FRotator HealthBarRotation = UKismetMathLibrary::FindLookAtRotation(HealthBarComponent->GetComponentLocation(),
		CameraLoc);
	if(HealthBarComponent){
		FRotator HealthBarPrevRotation = HealthBarComponent->GetComponentRotation();
    	//FRotator NewRotation = FRotator(HealthBarPrevRotation.Pitch,HealthBarRotation.Yaw,HealthBarPrevRotation.Roll);
    	HealthBarComponent->SetWorldRotation(HealthBarRotation);
	}	
}

void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseEnemy::PlayHitMontage(double Theta)
{
	FName HitName = "Front";
	if(FMath::IsWithin(Theta,-45, 45))
	{
		HitName = "Front";
	}
	else if(FMath::IsWithin(Theta,-135, -45))
	{
		HitName = "Left";
	}
	else if(FMath::IsWithin(Theta,45, 135))
	{
		HitName = "Right";
	}
	else
	{
		HitName = "Back";
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && HitMontage)
	{
		AnimInstance->Montage_Play(HitMontage);
		AnimInstance->Montage_JumpToSection(HitName,HitMontage);
	}
}

void ABaseEnemy::GetHit(FVector ImpactPoint)
{
	if(GetWorldTimerManager().TimerExists(HealthVisibilityHandle)){GetWorldTimerManager().ClearTimer(HealthVisibilityHandle);}
	HealthBarComponent->SetVisibility(true);
	GetWorldTimerManager().SetTimer(HealthVisibilityHandle,this,&ABaseEnemy::HideHealthBar,HeathBarVisibilityTime);
	PlayHitMontage(GetHitTheta(ImpactPoint));
	if(PainSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),PainSound,GetActorLocation());
	}
	if(BleedParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),BleedParticle,ImpactPoint);
	}
	
}

double ABaseEnemy::GetHitTheta( FVector HitImpactPoint)
{
	FVector ImpactPoint(HitImpactPoint.X,HitImpactPoint.Y,GetActorLocation().Z);
	FVector ToHit = (ImpactPoint - GetActorLocation()).GetSafeNormal();
	double CosTheta = FVector::DotProduct(ToHit,GetActorForwardVector());
	double Theta = FMath::RadiansToDegrees(FMath::Acos(CosTheta));
	FVector CrossProduct = FVector::CrossProduct(GetActorForwardVector(),ToHit);
	if(CrossProduct.Z < 0)
	{
		Theta *= -1;
	}
	LastHitAngle = Theta;
	//UE_LOG(LogTemp,Warning,TEXT("%f"),Theta);
	return Theta;
}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	//return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount,0, MaxHealth);
	if(CurrentHealth <= 0)
	{
		Die();
	}
	UHealthBar* HealthBar = Cast<UHealthBar>(HealthBarComponent->GetWidget());
	if(HealthBar)
	{

		float Perc = CurrentHealth/MaxHealth;
	
		HealthBar->SetHealthBar(Perc);
	}
	return CurrentHealth;
}

// Enemy Death
void ABaseEnemy::Die()
{
	ClearAllTimers(); // Clearing All activated timers
	EnemyAIController->GetBrainComponent()->StopLogic(TEXT("Enemy Died")); // Stopping AI logic
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // No Collision after death
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Disabling mesh collision for no response
	PlayWidgetAnimationsOnDeath(); // UI animation after enemy's death
	PlayDeathMontage(); // Montage for death animation
	GetWorldTimerManager().SetTimer
	(DeathTimerHandle,this,&ABaseEnemy::EnemyDestroy,DeathTime); // Destroy caller
}
void ABaseEnemy::EnemyDestroy()
{
	Destroy();
}


// Death Montage
/**
 * @brief There are 2 animation for death. This function will select 1 animation to play through montage.
 * Death pose is selected from @membervariable LastHitAngle
 */
void ABaseEnemy::PlayDeathMontage()
{
	TArray<FName> DeathAnim;
	DeathAnim.Add("DeathA");
	DeathAnim.Add("DeathB");
	int Index = 0;
	if(FMath::IsWithin(LastHitAngle,-45, 45))
	{
		Index  =0;
		
	}
	else if(FMath::IsWithin(LastHitAngle,-135, -45))
	{
		Index = FMath::RandRange(0,1);
		
	}
	else if(FMath::IsWithin(LastHitAngle,45, 135))
	{
		Index = FMath::RandRange(0,1);
	
	}
	else
	{
		Index = 1;
		
	}
	EnemyDeathState = CalculateDeathPose(Index);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
		AnimInstance->Montage_JumpToSection(DeathAnim[Index],DeathMontage);
	}
}
// Widget and Death
void ABaseEnemy::PlayWidgetAnimationsOnDeath()
{
	UHealthBar* HealthBar = Cast<UHealthBar>(HealthBarComponent->GetWidget());
	if(HealthBar)
	{
		HealthBar->OnEnemyDeath();
	}
}

// UI | Health bar
void ABaseEnemy::HideHealthBar()
{
	HealthBarComponent->SetVisibility(false);
}
// Enemy State
void ABaseEnemy::SetEnemyState(EEnemyState State)
{
	if(!EnemyAIController) return;
	EnemyState = State;
	if(EnemyState == EEnemyState::EES_Patrolling)
	{
		EnemyAIController->SetIsPatroling(true);
		
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
	else if(EnemyState == EEnemyState::EES_Chasing)
	{
		//EnemyAIController->SetIsLooking(false);
		EnemyChasing();
	}
	else if(EnemyState == EEnemyState::EES_Looking)
	{
		EnemyAIController->SetIsLooking(true);
	}
	else if(EnemyState == EEnemyState::EES_Attacking)
	{
		EnemyAIController->SetIsAttacking(true);
	}
}

void ABaseEnemy::EnemyChasing()
{
	if(GetWorldTimerManager().TimerExists(PatrolHandle))
	{
		GetWorldTimerManager().ClearTimer(PatrolHandle);
	}
	EnemyAIController->SetTargetToChase(TargetToChase);
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	EnemyAIController->SetIsChasing(true);
}


// Utility Functions
/**
 * @brief Just return Death pose state according to a @membervarible LastHitAngle. Index -> [0,1] 
 */
EDeathPose ABaseEnemy::CalculateDeathPose(int Index)
{
	if(Index == 0)
	{
		return EDeathPose::EDP_DeathA;
	}
	return EDeathPose::EDP_DeathB;	
}


/**
 *Ai
*/
//Patrolling
AActor* ABaseEnemy::GetNextPatrolPoint()
{
	if(PatrolPoints.Num() == 0 ) return nullptr;
	
	NewIndex = FMath::RandRange(0,PatrolPoints.Num() - 1);
	if(LastPatrolPoint)
	{
		PatrolPoints.AddUnique(LastPatrolPoint);
	}
	// while (NewIndex == LastIndex)
	// {
	// 	NewIndex = FMath::RandRange(0,PatrolPoints.Num() - 1);
	// }
	LastIndex = NewIndex;
	EnemyAIController->SetNextPatrolPoint(PatrolPoints[NewIndex]);
	AActor* Returner = PatrolPoints[NewIndex];
	PatrolPoints.Remove(Returner);
	LastPatrolPoint = Returner;
	return Returner;
	
	
	
}

// Chasing
/*Pawn sensing component call this component after every successful sensing interval */
/**
 * @brief: After successful sensing of @param: Pawn (player) we set @member: TargetToChase to Pawn
 * if enemy dont sense (or looses the Pawn/player) it will call @function: LookAround.
 */
void ABaseEnemy::OnSeeTarget(APawn* Pawn)
{
	AAurora* PlayerControlled = Cast<AAurora>(Pawn);
	if(PlayerControlled)
	{
		//UE_LOG(LogTemp,Error,TEXT("------------------------------------------------------------"));
		TargetToChase = Pawn;
		if(EnemyState != EEnemyState::EES_Chasing)
		{
			SetEnemyState(EEnemyState::EES_Chasing);
		}
		// Clearing Patrol timer if any exists.
		if(GetWorldTimerManager().TimerExists(PatrolHandle))
		{
			GetWorldTimerManager().ClearTimer(PatrolHandle);
		}
		// Below code is for checking if enemy looses the Pawn/Player then go to Looking state.
		if(GetWorldTimerManager().TimerExists(LookAroundStateHandle))
		{
			GetWorldTimerManager().ClearTimer(LookAroundStateHandle);
		}
		GetWorldTimerManager().SetTimer(LookAroundStateHandle,this,&ABaseEnemy::LookAround
			,PawnSensingComponent->SensingInterval * 4,false );
		
	}
	
}
/**
 * @brief Sets the enemy state to Looking and then after X wait time go to Patrolling state or to the chasing state
 * if Pawn/Player sensing successful.
 */
void ABaseEnemy::LookAround()
{
	SetEnemyState(EEnemyState::EES_Looking);
	GetWorldTimerManager().SetTimer(PatrolHandle,this,&ABaseEnemy::GoBackToPatrolling,LookAroundTime);
}

/**
 *@brief Enemy will go back to patrol state and regain its health
 */
void ABaseEnemy::GoBackToPatrolling()
{
	if(CurrentHealth <= 0) return;
	UE_LOG(LogTemp,Error,TEXT("GO back Called"))
	ClearAllTimers();
	
	CurrentHealth = MaxHealth;
	SetTargetToChase(nullptr);
	SetEnemyState(EEnemyState::EES_Patrolling);
	
}
/**
 * @brief Setting function TargetToChase
 */
void ABaseEnemy::SetTargetToChase(APawn* Target)
{
	TargetToChase = Target;
	if(EnemyAIController)
	{
		EnemyAIController->SetTargetToChase(TargetToChase);
	}
}


/**
 * @brief If Enemy goes out of range wrt it spawning point then it will go for patrolling again and regain its health.
 */
void ABaseEnemy::OnChasingRadiusEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor == this)
	{
		GoBackToPatrolling();
	}
}

void ABaseEnemy::ClearAllTimers()
{
	if(GetWorldTimerManager().TimerExists(LookAroundStateHandle))
	{
		GetWorldTimerManager().ClearTimer(LookAroundStateHandle);
	}
	if(GetWorldTimerManager().TimerExists(PatrolHandle))
	{
		GetWorldTimerManager().ClearTimer(PatrolHandle);
	}
}
