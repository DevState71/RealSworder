// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossPortalDirector.generated.h"

class UBoxComponent;
class ACameraActor;

UENUM(BlueprintType)
enum class EPortalSpawnShape : uint8
{
	Circle UMETA(DisplayName = "Circle"),
	Square UMETA(DisplayName = "Square"),
	Triangle UMETA(DisplayName = "Triangle"),
	Hexagon UMETA(DisplayName = "Hexagon"),
	Octagon UMETA(DisplayName = "Octagon")
};

UCLASS()
class SWORDER_API ABossPortalDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossPortalDirector();

	// Allows tick function to execute in editor viewport only
	virtual bool ShouldTickIfViewportsOnly() const override;
	virtual void Tick(float DeltaTime) override;

protected:
	// ==========================================
	// COMPONENTS
	// ==========================================
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* RootComp;

	// Scale this box in the editor to cover the boss room. 
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* ArenaTrigger;

	// ==========================================
	// SPATIAL CONFIGURATION
	// ==========================================
	UPROPERTY(EditAnywhere, Category = "Portal Configuration")
	TSubclassOf<AActor> PortalClassToSpawn;

	// This determines the geometric pattern that the portal will spawn along.
	UPROPERTY(EditAnywhere, Category = "Portal Configuration")
	EPortalSpawnShape SpawnShape = EPortalSpawnShape::Circle;

	UPROPERTY(EditAnywhere, Category = "Portal Configuration")
	float MinSpawnRadius = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Portal Configuration")
	float MaxSpawnRadius = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Portal Configuration")
	bool bCenterOnBossDeath = true;

	UPROPERTY(EditAnywhere, Category = "Portal Configuration")
	bool bProjectToNavMesh = true;

	UPROPERTY(EditAnywhere, Category = "Portal Configuration", meta = (MakeEditWidget = true))
	FVector PortalCenterOffset;

	UPROPERTY(EditAnywhere, Category = "Portal Configuration")
	bool bUseManualAnchors = false;

	UPROPERTY(EditAnywhere, Category = "Portal Configuration", meta = (MakeEditWidget = true, EditCondition = "bUseManualAnchors"))
	TArray<FVector> ManualPortalAnchors;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class UNiagaraSystem* PortalSpawnVFX;

	// ==========================================
	// CINEMATICS & PACING
	// ==========================================
	// Actors to physically block the exits. The Director locks them when the player enters, and unlocks them when the portal spawns.
	UPROPERTY(EditAnywhere, Category = "Cinematics")
	TArray<AActor*> LinkedArenaDoors;

	// A camera placed high in the room to cut to when the boss dies.
	UPROPERTY(EditAnywhere, Category = "Cinematics")
	ACameraActor* CinematicCamera;

	// How long to wait after the boss dies before the portal actually spawns.
	UPROPERTY(EditAnywhere, Category = "Cinematics")
	float SpawnDelay = 2.5f;

	// How long the camera takes to pan from the player to the Cinematic Camera (and back).
	UPROPERTY(EditAnywhere, Category = "Cinematics")
	float CameraBlendTime = 1.0f;

	// How long to hold the cinematic angle on the newly spawned portal before giving the player control back.
	UPROPERTY(EditAnywhere, Category = "Cinematics")
	float PostSpawnCameraHoldTime = 3.0f;

	FTimerHandle SequenceTimerHandle;

	// ==========================================
	// NATIVE FUNCTIONS
	// ==========================================
	// Helper function calculating exact coordinates for both spawning and editor drawing
	FVector CalculateShapeOffset(float Pct, float Radius) const;

	UFUNCTION()
	void OnPlayerEnterArena(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Internal steps for the timer sequence
	UFUNCTION()
	void ExecutePortalSpawn(FVector BossDeathLocation);

	UFUNCTION()
	void RestorePlayerControl();

public:	
	UFUNCTION(BlueprintCallable, Category = "Boss Encounter")
	void SpawnExitPortal(FVector BossDeathLocation);

#if WITH_EDITOR
	// Click this button in the Details Panel to test the procedural math without playing the game!
	UFUNCTION(CallInEditor, Category = "Portal Configuration")
	void TestSpawnLocation();
#endif
};
