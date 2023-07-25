// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Project_RPG/MainCharacter/Aurora.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	RootComponent = ItemMesh;
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SphereCollider->SetupAttachment(GetRootComponent());
	ItemInfo = CreateDefaultSubobject<UWidgetComponent>(TEXT("Item Infor Widget"));
	ItemInfo->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this,&AItem::OnBeginSphereCollider);
	SphereCollider->OnComponentEndOverlap.AddDynamic(this,&AItem::OnEndSphereCollider);
	ItemInfo->SetVisibility(false);
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AItem::OnBeginSphereCollider(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor) return;
	AAurora* AuroraRef = Cast<AAurora>(OtherActor);
	if(!AuroraRef) return;
	if(OtherComp == AuroraRef->GetRootComponent())
	{
		AuroraRef->AddOverlappingItems(this);
	}
}

void AItem::OnEndSphereCollider(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!OtherActor) return;
	AAurora* AuroraRef = Cast<AAurora>(OtherActor);
	if(!AuroraRef) return;
	if(OtherComp == AuroraRef->GetRootComponent())
	{
		AuroraRef->RemoveOverlappingItems(this);
	}
}
// Widget Showing
void AItem::ShowInfo(bool bShow)
{
	ItemInfo->SetVisibility(bShow);
	
}
