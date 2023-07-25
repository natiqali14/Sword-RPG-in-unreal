// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordAttack.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Project_RPG/Enemy/BaseEnemy.h"
#include "Project_RPG/Items/Weapons/Weapon.h"
#define ECC_CharMesh ECC_GameTraceChannel1
USwordAttack::USwordAttack()
{
	LastAttack = -1;
}

void USwordAttack::StartAbility(FGameplayTagContainer Blocked,AActor* PerformingActor, AController* Instigator)
{
	
	Super::StartAbility(Blocked, PerformingActor, Instigator);
	if(!CanStartAbilities(Blocked)) return;
	if(!PerformingActor) return;
	const ACharacter* CharacterInstigator = Cast<ACharacter>(Instigator->GetCharacter());
	
	if(CharacterInstigator)
	{
		GetOwningComponent()->SetGrantedTags(OnActivatedTags);
	
		PlayAbilityMontage(CharacterInstigator->GetMesh()->GetAnimInstance());
	}
	
	
}

void USwordAttack::StopAbility()
{
	
	Super::StopAbility();	
}

bool USwordAttack::CanStartAbilities(FGameplayTagContainer Blocked)
{
	if(!Super::CanStartAbilities(Blocked)) return false;
	return true;
}

void USwordAttack::InitiateAbilityMechanics(AActor* PerformingActor, AController* Instigator)
{
	//
	// if(!PerformingActor || !Instigator) return;
	// AWeapon* Sword = Cast<AWeapon>(PerformingActor);
	// if(!Sword) return;
	// TSet<AActor*> HitActors;
	// TArray<FHitResult> HitResults = SwordTrace(Sword);
	// for (auto HitResult : HitResults)
	// {
	// 	if(HitResult.GetActor() != Target) continue;;
	// 	
	// 	if(HitActors.Contains(Target)) return;
	// 	HitActors.Add(HitResult.GetActor());
	// 	
	// 	IHitInterface* HitActor = Cast<IHitInterface>(HitResult.GetActor());
	// 	if(HitActor)
	// 	{
	// 		HitActor->GetHit(HitResult.ImpactPoint);
	// 	}
	// 	UE_LOG(LogTemp,Warning,TEXT("%s"),*HitResult.GetActor()->GetFName().ToString());
	// 	UE_LOG(LogTemp,Warning,TEXT("Num of Set %d"),HitActors.Num());
	// 	UE_LOG(LogTemp,Warning,TEXT("Num of Hit Reuslts %d"),HitResults.Num());
	// 	
	// 	UGameplayStatics::ApplyDamage(HitResult.GetActor(),
	// 			Sword->GetWeaponInfo().WeaponDamage *AbilityProps.DamageMultiplier,Instigator,
	// 			PerformingActor,DamageType);
	// }
	SwordTraceCaller(PerformingActor,Instigator);
	
}

void USwordAttack::StopAbilityMechanics()
{
	Super::StopAbilityMechanics();
	if(GetWorld()->GetTimerManager().TimerExists(SwordTraceHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(SwordTraceHandle);
	}
	if(GetOwningComponent())
	{
		GetOwningComponent()->ResetHitActor(this);
	}
}

void USwordAttack::PlayAbilityMontage(UAnimInstance* AnimInstance)
{
	if(!AbilityMontage || MontageSectionName.Num() < 1 || !AnimInstance) return;
	LastAttack++;
	if(LastAttack > MontageSectionName.Num() -1){LastAttack = 0;}
	
	AnimInstance->Montage_Play(AbilityMontage);
	AnimInstance->Montage_JumpToSection(MontageSectionName[LastAttack],AbilityMontage);
	
	
}

TArray<FHitResult> USwordAttack::SwordTrace(AWeapon* SwordWeapon)
{
	TArray<FHitResult> HitResults;
	if(!SwordWeapon) return HitResults;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(SwordWeapon->GetOwner());
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_CharMesh));
	//ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Destructible));
	UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(),SwordWeapon->GetTraceStart()->GetComponentLocation(),
		SwordWeapon->GetTraceEnd()->GetComponentLocation(),SwordWeapon->GetTraceBoxExtent(),FRotator::ZeroRotator,ObjectTypes,false,
		IgnoredActors,EDrawDebugTrace::None,HitResults,true);
	return HitResults;
	
}



void USwordAttack::SwordTraceCaller(AActor* PerformingActor, AController* Instigator)
{
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this,FName("SwordTraceLoop"),PerformingActor, Instigator);
	GetWorld()->GetTimerManager().SetTimer(SwordTraceHandle,Delegate,GetWorld()->DeltaTimeSeconds,true);
}

void USwordAttack::SwordTraceLoop(AActor* PerformingActor, AController* Instigator)
{
	if(!PerformingActor || !Instigator) return;
	AWeapon* Sword = Cast<AWeapon>(PerformingActor);
	if(!Sword) return;
	TArray<FHitResult> HitResults = SwordTrace(Sword);
	for (auto HitActor : HitResults)
	{
		if(!GetOwningComponent()) return;
		if(GetOwningComponent()->HitActorFound(this, HitActor.GetActor())) return;
		GetOwningComponent()->AddHitActor(this,HitActor.GetActor());
		IHitInterface* HitObj = Cast<IHitInterface>(HitActor.GetActor());
		if(HitObj)
		{
			HitObj->GetHit(HitActor.ImpactPoint);
			UGameplayStatics::ApplyDamage(HitActor.GetActor(),
				Sword->GetWeaponInfo().WeaponDamage *AbilityProps.DamageMultiplier,Instigator,
				PerformingActor,DamageType);
			UE_LOG(LogTemp, Error, TEXT("Damage: %f"),Sword->GetWeaponInfo().WeaponDamage *AbilityProps.DamageMultiplier)
		}
		
	}
}