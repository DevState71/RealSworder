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

	InitialLifeSpan = 0.1f; // Set lifespan to 0.1 seconds

}

// Called when the game starts or when spawned
void AElementalAOE::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->SetSphereRadius(Radius);

	
}

// Called every frame
void AElementalAOE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AElementalAOE::ActivateAOE()
{
	if(!IsValid(CollisionSphere))
	{UE_LOG(LogTemp, Warning, TEXT("CollisionSphere is not valid in ActivateAOE"));
	return;
	}

	CollisionSphere->SetSphereRadius(Radius);
	CollisionSphere->UpdateOverlaps();

	TArray<AActor*> OverlappingActors;
	CollisionSphere->GetOverlappingActors(OverlappingActors);
	if (ComboSource)
	{
		UE_LOG(LogTemp, Warning, TEXT("ComboSource is valid: %s"), *ComboSource->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ComboSource is null"));
	}


	for (AActor* OverlappingActor : OverlappingActors)
	{

		if (IsValid(OverlappingActor))
		{
			if (OverlappingActor == ComboSource)
			{
				continue;
			}
			else
			{
				HitActors.Add(OverlappingActor);
				UE_LOG(LogTemp, Warning, TEXT("Processing Overlapping Actor: %s"), *OverlappingActor->GetName());
				ProcessOverlap(OverlappingActor, StatusToApply);
			}
		}
	}

}
void AElementalAOE::InitializeAOE(AActor* InEffectSource, AActor* InComboSource, float InDamage, float InRadius, FGameplayTag InStatusToApply)
{
	EffectSource = InEffectSource;
	Damage = InDamage;
	Radius = InRadius;
	StatusToApply = InStatusToApply;
	ComboSource = InComboSource;

	UE_LOG(LogTemp, Warning, TEXT("AOE Initialized with EffectSource: %s, ComboSource: %s, Damage: %f, Radius: %f, StatusToApply: %s"), 
		*GetNameSafe(EffectSource), 
		*GetNameSafe(ComboSource), 
		Damage, 
		Radius, 
		*StatusToApply.ToString());

	ActivateAOE();
}

void AElementalAOE::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ProcessOverlap(OtherActor, StatusToApply);
}

void AElementalAOE::ProcessOverlap(AActor* OtherActor, FGameplayTag InStatusToApply)
{
	if(OtherActor == ComboSource)
	{
		return;
	}

	for(AActor* Actor : HitActors)
	{
		if (Actor == OtherActor)
		{
			return; // Already processed this actor
		}
	}

	if (InStatusToApply == FGameplayTag::RequestGameplayTag("Status.Ember"))
	{
		UHealthComponent* HealthComp = OtherActor->FindComponentByClass<UHealthComponent>();

		if (HealthComp)
		{
			HealthComp->TakeFlatDamage(Damage, EffectSource);
		}
	}
}

