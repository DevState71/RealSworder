// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FormationTrapPlate.generated.h"

class USplineComponent;
class UNiagaraSystem;
class UBoxComponent;

UENUM(BlueprintType)
enum class EFormationShape : uint8
{
	Circle UMETA(DisplayName = "Circle"),
	Square UMETA(DisplayName = "Square"),
	Triangle UMETA(DisplayName = "Triangle"),
	Line UMETA(DisplayName = "Line"),
	Cross UMETA(DisplayName = "Cross"),
	TwoLines UMETA(DisplayName = "Two Lines"),
	Arc UMETA(DisplayName = "Arc"),
	Wedge UMETA(DisplayName = "Wedge"),
	Pentagon UMETA(DisplayName = "Pentagon"),
	Hexagon UMETA(DisplayName = "Hexagon"),
	Octagon UMETA(DisplayName = "Octagon"),
	Random UMETA(DisplayName = "Random"),
	Spline UMETA(DisplayName = "Spline Path")
};

USTRUCT(BlueprintType)
struct FFormationWave
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Wave Definition")
	TSubclassOf<AActor> EnemyClassToSpawn;

	UPROPERTY(EditAnywhere, Category = "Wave Definition")
	TSubclassOf<AActor> LeaderClassToSpawn;

	UPROPERTY(EditAnywhere, Category = "Wave Definition", meta = (ClampMin = "0"))
	int32 LeaderIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Wave Definition", meta = (ClampMin = "1", UIMin = "1", ClampMax = "50", UIMax = "50"))
	int32 EnemyCount = 8;

	UPROPERTY(EditAnywhere, Category = "Wave Definition")
	EFormationShape SelectedShape = EFormationShape::Circle;

	UPROPERTY(EditAnywhere, Category = "Wave Definition", meta = (ClampMin = "3", UIMin = "3", ClampMax = "8", UIMax = "8", EditCondition = "SelectedShape == EFormationShape::Cross", EditConditionHides))
	int32 CrossProngCount = 4;

	UPROPERTY(EditAnywhere, Category = "Wave Definition")
	float FormationRadius = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Wave Definition", meta = (UIMin = "0.0", UIMax = "360.0"))
	float FormationRotation = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Wave Definition")
	bool bFacePlayer = true;

	UPROPERTY(EditAnywhere, Category = "Wave Definition", meta = (EditCondition = "!bFacePlayer", MakeEditWidget = true))
	FVector CustomFacingPoint = FVector(500.0f, 0.0f, 0.0f);

	// How long the warning telegraph lingers before the first enemy actually spawns
	UPROPERTY(EditAnywhere, Category = "Wave Definition")
	float TelegraphDelay = 1.5f;

	// Delay between individual enemy spawns within THIS wave
	UPROPERTY(EditAnywhere, Category = "Wave Timing", meta = (ClampMin = "0.0"))
	float SpawnDelay = 0.1f;

	// How long to wait AFTER this wave finishes spawning before starting the NEXT wave in the array
	UPROPERTY(EditAnywhere, Category = "Wave Timing", meta = (ClampMin = "0.0"))
	float DelayBeforeNextWave = 3.0f;
};

UCLASS()
class SWORDER_API AFormationTrapPlate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFormationTrapPlate();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Tells Unreal to run the Tick function in the Editor viewport
	virtual bool ShouldTickIfViewportsOnly() const override;

#if WITH_EDITOR
	// Fires automatically when a property is changed in the Details panel
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ===============================
	// NATIVE OBJECT POOLING
	// ===============================

	// Flat array to hold per-spawned enemies, so they do not hog up memory space
	UPROPERTY()
	TArray<AActor*> PooledEnemies;

	// Tallies up the required enemies and spawns them under the map when the level loads
	void InitializeObjectPool();

	// Pulls a sleeping enemy from the array, or spawns an emergency backup if the pool is empty
	AActor* GetEnemyFromPool(TSubclassOf<AActor> EnemyClass);

	// -------------------------------

	// Spline used for custom drawn ambush paths
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USplineComponent* AmbushSpline;

	// Add as many waves as you want here! They will spawn sequentially.
	UPROPERTY(EditAnywhere, Category = "Spawning|Waves")
	TArray<FFormationWave> Waves;

#if WITH_EDITORONLY_DATA
	// Selects which wave to preview in the editor. Set to -1 to view ALL waves at once.
	UPROPERTY(EditAnywhere, Category = "Spawning|Editor", meta = (ClampMin = "-1"))
	int32 PreviewWaveIndex = 0;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerZone;

	// The center point of the formation relative to the trap plate. 
	// MakeEditWidget creates a draggable 3D diamond in the editor viewport!
	UPROPERTY(EditAnywhere, Category = "Spawning|Layout", meta = (MakeEditWidget = true))
	FVector FormationCenterOffset = FVector::ZeroVector;

	// Particle burst to hide the pop-in
	UPROPERTY(EditAnywhere, Category = "Spawning|Effects")
	UNiagaraSystem* SpawnVFX;

	// Sound effect on spawn
	UPROPERTY(EditAnywhere, Category = "Spawning|Effects")
	USoundBase* SpawnSound;

	// VFX For Pre-Spawn Warning Telegraph
	UPROPERTY(EditAnywhere, Category = "Spawning|Effects")
	UNiagaraSystem* TelegraphVFX;

	// Actual spawn execution function that fires after the telegraph finishes
	void ExecuteSpawn(TSubclassOf<AActor> ClassToSpawn, FVector SpawnLocation, FRotator SpawnRotation);

	UPROPERTY(EditAnywhere, Category = "Spawning|Navigator")
	bool bProjectToNavMesh = true;

	UPROPERTY(EditAnywhere, Category = "Spawning|Navigation", meta = (EditCondition = "bProjectToNavMesh"))
	FVector NavSearchExtent = FVector(250.0f, 250.0f, 500.0f);

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:
	// Core flow functions
	void StartNextWave();

	// Helper function to calculate offsets based on the chosen shape
	TArray<FVector> CalculateFormationOffsets(const FFormationWave& Wave) const;

	// Timer logic
	UFUNCTION()
	void SpawnNextEnemy();

	FTimerHandle SpawnTimerHandle;
	FTimerHandle WaveTimerHandle;

	int32 CurrentWaveIndex = 0;
	int32 CurrentSpawnIndex = 0;
	TArray<FVector> CachedSpawnLocations;

#if WITH_EDITOR
	// Helper function to calculate the nearest valid interval
	void EnforceShapeRules();
#endif
};
