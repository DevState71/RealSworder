// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/SpikePlate.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h" 
#include "TimerManager.h"

// Sets default values
ASpikePlate::ASpikePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
	RootComponent = TriggerZone;
	TriggerZone->SetCollisionProfileName(TEXT("Trigger"));

	SpikeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpikeMesh"));
	SpikeMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASpikePlate::BeginPlay()
{
	Super::BeginPlay();
	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &ASpikePlate::OnOverlapBegin);
	TriggerZone->OnComponentEndOverlap.AddDynamic(this, &ASpikePlate::OnOverlapEnd);
}

void ASpikePlate::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        ActorsOnPlate.AddUnique(OtherActor);

        if (!GetWorldTimerManager().IsTimerActive(DamageTimerHandle))
        {
            DealDamage();
            GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &ASpikePlate::DealDamage, TickInterval, true);
        }
    }
}

void ASpikePlate::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor)
    {
        ActorsOnPlate.Remove(OtherActor);

        if (ActorsOnPlate.IsEmpty()) { GetWorldTimerManager().ClearTimer(DamageTimerHandle); }
    }
}

void ASpikePlate::DealDamage()
{
    for (int32 i = ActorsOnPlate.Num() - 1; i >= 0; --i)
    {
        AActor* Target = ActorsOnPlate[i];
        if (IsValid(Target))
        {
            // Construct a basic damage event
            FDamageEvent DamageEvent;

            // Call TakeDamage directly on the actor.
            Target->TakeDamage(DamagePerTick, DamageEvent, nullptr, this);
        }
        else { ActorsOnPlate.RemoveAt(i); }
    }
}
