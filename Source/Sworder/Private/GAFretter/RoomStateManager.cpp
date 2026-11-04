// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/RoomStateManager.h"
#include "GAFretter/FormationTrapPlate.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ARoomStateManager::ARoomStateManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Initialize the trigger volume and set it as the root
    RoomTriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("RoomTriggerVolume"));
    RootComponent = RoomTriggerVolume;

    // Standardize the collision so it only overlaps physics bodies/pawns
    RoomTriggerVolume->SetCollisionProfileName(TEXT("Trigger"));

    CurrentRoomState = ERoomState::Idle;
    TotalActiveEnemies = 0;
}

// Called when the game starts or when spawned
void ARoomStateManager::BeginPlay()
{
	Super::BeginPlay();
	
    // Bind the overlap event at runtime
    if (RoomTriggerVolume)
    {
        RoomTriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ARoomStateManager::OnPlayerEntered);
    }
}

void ARoomStateManager::OnPlayerEntered(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Ensure the room is idle, the actor is valid, and the actor is a Character (the player)
    if (CurrentRoomState == ERoomState::Idle && OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
    {
        CurrentRoomState = ERoomState::Lockdown;
        InitiateLockdown();
    }
}

void ARoomStateManager::InitiateLockdown()
{
    // 1. Lock the doors (This assumes standard blockout cubes that turn solid/visible)
    for (AActor* Door : LinkedDoors)
    {
        if (Door)
        {
            Door->SetActorHiddenInGame(false);
            Door->SetActorEnableCollision(true);
        }
    }

    // 2. Trigger the ambushes
    for (AFormationTrapPlate* Spawner : RoomSpawners)
    {
        if (Spawner)
        {
            // First, get the number of enemies this specific plate will spawn and add it to our tracking pool
            TotalActiveEnemies += Spawner->GetTotalEnemiesToSpawn();

            // Next, bind our Room Manager's 'OnEnemyDefeated' function to the Trap Plate's delegate 
            // so we are notified every time one of its enemies dies.
            Spawner->OnTrapPlateEnemyDefeated.AddDynamic(this, &ARoomStateManager::OnEnemyDefeated);

            // Finally, trigger the plate to start its spline math and pre-spawn telegraphing
            Spawner->ActivateTrapPlate();
        }
    }
}

void ARoomStateManager::OnEnemyDefeated()
{
    // Prevent math errors if an enemy somehow dies from poison/fire after the room is cleared
    if (CurrentRoomState != ERoomState::Lockdown) return;

    TotalActiveEnemies--;

    // Room is fully cleared
    if (TotalActiveEnemies <= 0)
    {
        ResolveRoom();
    }
}

void ARoomStateManager::ResolveRoom()
{
    CurrentRoomState = ERoomState::Cleared;

    // 1. Unlock the doors
    for (AActor* Door : LinkedDoors)
    {
        if (Door)
        {
            Door->SetActorHiddenInGame(true);
            Door->SetActorEnableCollision(false);
        }
    }
}

// Called every frame
//void ARoomStateManager::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

