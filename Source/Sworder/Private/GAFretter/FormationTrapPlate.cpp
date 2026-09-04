// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/FormationTrapPlate.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AFormationTrapPlate::AFormationTrapPlate()
{
    PrimaryActorTick.bCanEverTick = true;
	TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
	RootComponent = TriggerZone;
	TriggerZone->SetCollisionProfileName(TEXT("Trigger"));

}

// Called when the game starts or when spawned
void AFormationTrapPlate::BeginPlay()
{
	Super::BeginPlay();
	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AFormationTrapPlate::OnOverlap);
}

bool AFormationTrapPlate::ShouldTickIfViewportsOnly() const
{
    // Allows the Tick function to execute in the editor viewport
    return true;
}

void AFormationTrapPlate::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

#if WITH_EDITOR
    UWorld* World = GetWorld();

    // Only draw in the editor viewport before the game actually starts
    if (World && !HasActorBegunPlay())
    {
        FVector Center = GetActorLocation();
        EFormationShape PreviewShape = SelectedShape;

        // If Random is selected, preview it as a Circle in the editor
        if (PreviewShape == EFormationShape::Random)
        {
            PreviewShape = EFormationShape::Circle;
        }

        TArray<FVector> Offsets = CalculateFormationOffsets(PreviewShape);

        for (const FVector& Offset : Offsets)
        {
            FVector SpawnLoc = Center + Offset;
            // Draw a red sphere at the spawn point
            DrawDebugSphere(World, SpawnLoc, 30.0f, 12, FColor::Red, false, -1.0f, 0, 2.0f);
            // Draw a green line connecting it back to the trigger plate center
            DrawDebugLine(World, Center, SpawnLoc, FColor::Green, false, -1.0f, 0, 1.0f);
        }
    }
#endif
}

void AFormationTrapPlate::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Ensure we have a valid actor, a valid class to spawn, and at least 1 enemy requested
    if (OtherActor && OtherActor != this && EnemyClassToSpawn && EnemyCount > 0)
    {
        // Disable the trigger so it only fires once per level
        TriggerZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        FVector Center = GetActorLocation();

        EFormationShape ShapeToSpawn = SelectedShape;
        if (ShapeToSpawn == EFormationShape::Random)
        {
            // Automatically scales the random pool based on how many enum values exist before 'Random'
            uint8 MaxShapeIndex = static_cast<uint8>(EFormationShape::Random) - 1;
            ShapeToSpawn = static_cast<EFormationShape>(FMath::RandRange(0, MaxShapeIndex));
        }

        TArray<FVector> Offsets = CalculateFormationOffsets(ShapeToSpawn);

        for (const FVector& Offset : Offsets)
        {
            FVector SpawnLocation = Center + Offset;
            // Spawn the actor at the calculated world location
            GetWorld()->SpawnActor<AActor>(EnemyClassToSpawn, SpawnLocation, FRotator::ZeroRotator);
        }
    }
}

TArray<FVector> AFormationTrapPlate::CalculateFormationOffsets(EFormationShape Shape) const
{
    TArray<FVector> Offsets;
    if (EnemyCount <= 0) return Offsets;

    for (int32 i = 0; i < EnemyCount; ++i)
    {
        // Angle step size based on total enemy count for circular shapes
        float Angle = (TWO_PI / (float)EnemyCount) * i;
        float XOffset = 0.0f;
        float YOffset = 0.0f;

        switch (Shape)
        {
        case EFormationShape::Circle:
            XOffset = FMath::Cos(Angle) * FormationRadius;
            YOffset = FMath::Sin(Angle) * FormationRadius;
            break;

        case EFormationShape::Square:
        {
            // Dynamic square: calculates perimeter position based on percentage
            float PerimeterPct = (float)i / EnemyCount;
            if (PerimeterPct < 0.25f) { // Top edge
                XOffset = FormationRadius;
                YOffset = FMath::Lerp(-FormationRadius, FormationRadius, PerimeterPct * 4.0f);
            }
            else if (PerimeterPct < 0.5f) { // Right edge
                XOffset = FMath::Lerp(FormationRadius, -FormationRadius, (PerimeterPct - 0.25f) * 4.0f);
                YOffset = FormationRadius;
            }
            else if (PerimeterPct < 0.75f) { // Bottom edge
                XOffset = -FormationRadius;
                YOffset = FMath::Lerp(FormationRadius, -FormationRadius, (PerimeterPct - 0.5f) * 4.0f);
            }
            else { // Left edge
                XOffset = FMath::Lerp(-FormationRadius, FormationRadius, (PerimeterPct - 0.75f) * 4.0f);
                YOffset = -FormationRadius;
            }
        }
        break;

        case EFormationShape::Triangle:
        {
            int32 Side = i % 3;
            float Progress = (float)(i / 3) / FMath::Max(1, (EnemyCount / 3));
            if (Side == 0) {
                XOffset = 0.0f;
                YOffset = FMath::Lerp(FormationRadius, -FormationRadius, Progress);
            }
            else if (Side == 1) {
                XOffset = FMath::Lerp(0.0f, FormationRadius, Progress);
                YOffset = FMath::Lerp(FormationRadius, -FormationRadius, Progress);
            }
            else {
                XOffset = FMath::Lerp(0.0f, -FormationRadius, Progress);
                YOffset = FMath::Lerp(FormationRadius, -FormationRadius, Progress);
            }
        }
        break;

        case EFormationShape::Line:
            // Spaced evenly along the X axis, centered on the trap plate
            XOffset = (i - ((EnemyCount - 1) / 2.0f)) * (FormationRadius * 0.75f);
            break;

        case EFormationShape::Cross:
        {
            // 4 arms spreading outward
            int32 Arm = i % 4;
            int32 Depth = (i / 4) + 1;
            XOffset = FMath::Cos((TWO_PI / 4.0f) * Arm) * (FormationRadius * 0.5f * Depth);
            YOffset = FMath::Sin((TWO_PI / 4.0f) * Arm) * (FormationRadius * 0.5f * Depth);
        }
        break;

        case EFormationShape::TwoLines:
        {
            // Splits the count in half for two parallel rows
            int32 HalfCount = FMath::CeilToInt((float)EnemyCount / 2.0f);
            bool bTopRow = (i < HalfCount);
            int32 RowIndex = bTopRow ? i : (i - HalfCount);
            XOffset = (RowIndex - ((HalfCount - 1) / 2.0f)) * (FormationRadius * 0.75f);
            YOffset = bTopRow ? (FormationRadius * 0.5f) : -(FormationRadius * 0.5f);
        }
        break;

        case EFormationShape::Arc:
        {
            // Spaces the enemies across a 180-degree semi-circle
            float ArcAngle = (PI / FMath::Max(1.0f, (float)EnemyCount - 1.0f)) * i;
            XOffset = FMath::Cos(ArcAngle) * FormationRadius;
            YOffset = FMath::Sin(ArcAngle) * FormationRadius;
        }
        break;

        case EFormationShape::Wedge:
            if (i == 0) {
                XOffset = 0.0f;
                YOffset = FormationRadius;
            }
            else {
                // One leader at the front, alternating followers forming the V wings
                int32 Row = (i + 1) / 2;
                float Side = (i % 2 == 0) ? 1.0f : -1.0f;
                XOffset = Side * Row * (FormationRadius * 0.4f);
                YOffset = FormationRadius - (Row * (FormationRadius * 0.5f));
            }
            break;

        case EFormationShape::Random:
            // Fallback to prevent compiler warnings, though logic prevents reaching here
            break;
        }
        Offsets.Add(FVector(XOffset, YOffset, 0.0f));
    }
    return Offsets;
}

