// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FormationTrapPlate.generated.h"

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerZone;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> EnemyClassToSpawn;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float FormationRadius = 500.0f;

	// The number of enemies to spawn in the formation
	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = "1", UIMin = "1"))
	int32 EnemyCount = 8;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	EFormationShape SelectedShape = EFormationShape::Circle;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:
	// Helper function to calculate offsets based on the chosen shape
	TArray<FVector> CalculateFormationOffsets(EFormationShape Shape) const;
};
