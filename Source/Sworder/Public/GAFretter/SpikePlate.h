// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpikePlate.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class SWORDER_API ASpikePlate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpikePlate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // The physical trigger volume players step on
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* TriggerZone;

    // The visual mesh for the spikes
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* SpikeMesh;

    // How much damage to apply each tick
    UPROPERTY(EditAnywhere, Category = "Damage")
    float DamagePerTick = 10.0f;

    // How often (in seconds) the damage applies
    UPROPERTY(EditAnywhere, Category = "Damage")
    float TickInterval = 0.5f;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:	
    // Function that runs every tick interval
    void DealDamage();

    FTimerHandle DamageTimerHandle;

    // Tracks all actors currently standing on the plate
    UPROPERTY()
    TArray<AActor*> ActorsOnPlate;
};
