// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ElementalAOE.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AElementalAOE::AElementalAOE()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));

	SetRootComponent(CollisionSphere);

	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionSphere->SetGenerateOverlapEvents(true);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AElementalAOE::OnSphereOverlapBegin);	

}

// Called when the game starts or when spawned
void AElementalAOE::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->SetSphereRadius(Radius);

	TArray<AActor*> OverlappingActors;
	CollisionSphere->GetOverlappingActors(OverlappingActors);

	for(AActor* OverlappingActor : OverlappingActors)
	{
		if(IsValid(OverlappingActor))
		{
			if(OverlappingActor == ComboSource)
			{
				continue;
			}
			else
			{
				ProcessOverlap(OverlappingActor, StatusToApply);
			}
		}
	}
	
}

// Called every frame
void AElementalAOE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AElementalAOE::InitializeAOE(AActor* InEffectSource, AActor* InComboSource, float InDamage, float InRadius, FGameplayTag InStatusToApply)
{
	EffectSource = InEffectSource;
	Damage = InDamage;
	Radius = InRadius;
	StatusToApply = InStatusToApply;
	ComboSource = InComboSource;
}

void AElementalAOE::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ProcessOverlap(OtherActor, StatusToApply);
}

void AElementalAOE::ProcessOverlap(AActor* OtherActor, FGameplayTag InStatusToApply)
{
	if (InStatusToApply == FGameplayTag::RequestGameplayTag("Status.Ember"))
	{
		UHealthComponent* HealthComp = OtherActor->FindComponentByClass<UHealthComponent>();

		if (HealthComp)
		{
			HealthComp->TakeFlatDamage(Damage, EffectSource);
		}
	}
}

