// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomStateManager.generated.h"

// Forward declarations to keep compile times fast
class UBoxComponent;
class AFormationTrapPlate;

// Strongly typed Enum to track the room's current sequence phase
UENUM(BlueprintType)
enum class ERoomState : uint8
{
	Idle        UMETA(DisplayName = "Waiting For Player"),
	Lockdown    UMETA(DisplayName = "Combat Active"),
	Cleared     UMETA(DisplayName = "Room Conquered")
};

UCLASS()
class SWORDER_API ARoomStateManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomStateManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // The physical bounds of the room that trigger the lockdown
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room Components")
    UBoxComponent* RoomTriggerVolume;

    // Current state of the room to prevent double-triggering
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room State")
    ERoomState CurrentRoomState;

    // Arrays for the designer to assign physical door actors in the editor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Setup")
    TArray<AActor*> LinkedDoors;

    // Arrays for the designer to assign trap plates/spawners inside the room
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Setup")
    TArray<AFormationTrapPlate*> RoomSpawners;

    // Internal tracking variable for how many enemies are currently alive
    int32 TotalActiveEnemies;

    // Core Logic Functions
    void InitiateLockdown();
    void ResolveRoom();

    // Delegate Binding for the player entering the room's trigger box
    UFUNCTION()
    void OnPlayerEntered(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // Delegate Binding to track when an spawned enemy dies
    UFUNCTION()
    void OnEnemyDefeated();

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

};
