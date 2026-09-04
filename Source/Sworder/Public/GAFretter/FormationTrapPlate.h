// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FormationTrapPlate.generated.h"

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
	Random UMETA(DisplayName = "Random")
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerZone;

	UPROPERTY(EditAnywhere, Category = "Spawning|Classes")
	TSubclassOf<AActor> EnemyClassToSpawn;

	// Spawns at the very front/center (Index 0)
	UPROPERTY(EditAnywhere, Category = "Spawning|Classes")
	TSubclassOf<AActor> LeaderClassToSpawn;

	// The specific index in the formation that spawns the leader (0 is the first spawn)
	UPROPERTY(EditAnywhere, Category = "Spawning|Classes", meta = (ClampMin = "0"))
	int32 LeaderIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Spawning|Layout")
	float FormationRadius = 500.0f;

	// The rotation of the entire formation (in degrees)
	UPROPERTY(EditAnywhere, Category = "Spawning|Layout", meta = (UIMin = "0.0", UIMax = "360.0"))
	float FormationRotation = 0.0f;

	// The center point of the formation relative to the trap plate. 
	// MakeEditWidget creates a draggable 3D diamond in the editor viewport!
	UPROPERTY(EditAnywhere, Category = "Spawning|Layout", meta = (MakeEditWidget = true))
	FVector FormationCenterOffset = FVector::ZeroVector;

	// The number of enemies to spawn in the formation
	UPROPERTY(EditAnywhere, Category = "Spawning|Layout", meta = (ClampMin = "1", UIMin = "1", ClampMax = "50", UIMax = "50"))
	int32 EnemyCount = 8;

	UPROPERTY(EditAnywhere, Category = "Spawning|Layout")
	EFormationShape SelectedShape = EFormationShape::Circle;

	// Dynamically adjusts the number of arms on the Cross (Hides if Cross is not selected)
	UPROPERTY(EditAnywhere, Category = "Spawning|Layout", meta = (ClampMin = "3", UIMin = "3", ClampMax = "8", UIMax = "8", EditCondition = "SelectedShape == EFormationShape::Cross", EditConditionHides))
	int32 CrossProngCount = 4;

	// Delay between each spawn to prevent frame hitches
	UPROPERTY(EditAnywhere, Category = "Spawning|Effects", meta = (ClampMin = "0.0"))
	float SpawnDelay = 0.1f;

	// Particle burst to hide the pop-in
	UPROPERTY(EditAnywhere, Category = "Spawning|Effects")
	UNiagaraSystem* SpawnVFX;

	// Sound effect on spawn
	UPROPERTY(EditAnywhere, Category = "Spawning|Effects")
	USoundBase* SpawnSound;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:
	// Helper function to calculate offsets based on the chosen shape
	TArray<FVector> CalculateFormationOffsets(EFormationShape Shape) const;

	// Timer logic
	UFUNCTION()
	void SpawnNextEnemy();

	FTimerHandle SpawnTimerHandle;
	int32 CurrentSpawnIndex = 0;
	TArray<FVector> CachedSpawnLocations;

#if WITH_EDITOR
	// Helper function to calculate the nearest valid interval
	void EnforceShapeRules();
#endif
};
