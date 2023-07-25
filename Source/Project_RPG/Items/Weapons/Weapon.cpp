// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Project_RPG/MainCharacter/Aurora.h"

AWeapon::AWeapon()
{
	WeaponTrigger = CreateDefaultSubobject<UBoxComponent>("Weapon Trigger");
	WeaponTrigger->SetupAttachment(GetRootComponent());
	WeaponTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	TraceStart = CreateDefaultSubobject<USceneComponent>("Trace Start");
	TraceStart->SetupAttachment(GetRootComponent());
	
	TraceEnd = CreateDefaultSubobject<USceneComponent>("Trace End");
	TraceEnd->SetupAttachment(GetRootComponent());

	TraceBoxExtent = FVector(2.5,2.5,2.5);
	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponTrigger->OnComponentBeginOverlap.AddDynamic(this,&AWeapon::WeaponTriggerBegin);
	WeaponTrigger->OnComponentEndOverlap.AddDynamic(this,&AWeapon::WeaponTriggerEnd);
}

void AWeapon::Equip(USceneComponent* Parent, FAttachmentTransformRules Rules, FName SocketName)
{
	GetItemMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetSphereCollider()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
;
	GetItemMesh()->AttachToComponent(Parent,Rules,SocketName);
	
}

void AWeapon::IsAttacking(bool bAttack)
{
	if(bAttack)
	{
		WeaponTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		TriggeredActors = TSet<AActor*>();
		
		WeaponTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// void AWeapon::SwordTrace()
// {
// 	TArray<FHitResult> HitResults;
// 	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
// 	TArray<AActor*> IgnoredActors;
// 	IgnoredActors.Add(GetOwner());
// 	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
// 	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
// 	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Destructible));
// 	UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(),TraceStart->GetComponentLocation(),
// 		TraceEnd->GetComponentLocation(),TraceBoxExtent,FRotator::ZeroRotator,ObjectTypes,false,
// 		IgnoredActors,EDrawDebugTrace::Persistent,HitResults,true);
// 	for (auto HitResult : HitResults)
// 	{
// 		if(HitResult.bBlockingHit)
// 		{
// 			
// 			//1. Apply Damage
// 			//2. Some Hitting VFX
// 			//3. Sound
// 			
// 		}
// 	}
// 	
// }

void AWeapon::WeaponTriggerBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(TriggeredActors.Contains(OtherActor)) return;
	AAurora* Character = Cast<AAurora>(GetOwner());
	if(Character == OtherActor) return;
	
	if(Character)
	{
		//UE_LOG(LogTemp,Warning,TEXT("%s"),*OtherActor->GetFName().ToString());
		TriggeredActors.Add(OtherActor);
		//Character->InitiateCurrentAbilityMechanics(OtherActor);
		
	}
}

void AWeapon::WeaponTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
