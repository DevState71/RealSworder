// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/FormationTrapPlate.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"

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

// Allows the Tick function to execute in the editor viewport
bool AFormationTrapPlate::ShouldTickIfViewportsOnly() const { return true; }

void AFormationTrapPlate::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

#if WITH_EDITOR
    UWorld* World = GetWorld();

    // Only draw in the editor viewport before the game actually starts
    if (World && !HasActorBegunPlay())
    {
        // Transforms the draggable local offset into the correct world position
        FVector Center = GetTransform().TransformPosition(FormationCenterOffset);
        EFormationShape PreviewShape = SelectedShape;

        // If Random is selected, preview it as a Circle in the editor
        if (PreviewShape == EFormationShape::Random)
        {
            PreviewShape = EFormationShape::Circle;
        }

        TArray<FVector> Offsets = CalculateFormationOffsets(PreviewShape);

        for (int32 i = 0; i < Offsets.Num(); ++i)
        {
            FVector SpawnLoc = Center + Offsets[i];
            // Highlight the Leader spawn position in Yellow if a Leader class is assigned
            FColor SphereColor = (i == LeaderIndex && LeaderClassToSpawn != nullptr) ? FColor::Yellow : FColor::Red;
            // Draw a red sphere at the spawn point
            DrawDebugSphere(World, SpawnLoc, 30.0f, 12, SphereColor, false, -1.0f, 0, 2.0f);
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
        // Transforms the draggable local offset into the correct world position
        FVector Center = GetTransform().TransformPosition(FormationCenterOffset);

        EFormationShape ShapeToSpawn = SelectedShape;
        if (ShapeToSpawn == EFormationShape::Random)
        {
            // Automatically scales the random pool based on how many enum values exist before 'Random'
            uint8 MaxShapeIndex = static_cast<uint8>(EFormationShape::Random) - 1;
            ShapeToSpawn = static_cast<EFormationShape>(FMath::RandRange(0, MaxShapeIndex));
        }

        TArray<FVector> Offsets = CalculateFormationOffsets(ShapeToSpawn);

        CachedSpawnLocations.Empty();
        for (const FVector& Offset : Offsets) { CachedSpawnLocations.Add(Center + Offset); }

        CurrentSpawnIndex = 0;

        // Kick off the staggered spawn loop. (0.0f starts the first one instantly)
        GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AFormationTrapPlate::SpawnNextEnemy, SpawnDelay, true, 0.0f);
    }
}

void AFormationTrapPlate::SpawnNextEnemy()
{
    if (CurrentSpawnIndex >= CachedSpawnLocations.Num())
    {
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
        return;
    }

    FVector SpawnLocation = CachedSpawnLocations[CurrentSpawnIndex];

    // Determine if this index gets the Leader class or a Grunt
    TSubclassOf<AActor> ClassToSpawn = EnemyClassToSpawn;
    if (CurrentSpawnIndex == LeaderIndex && LeaderClassToSpawn != nullptr)
    {
        ClassToSpawn = LeaderClassToSpawn;
    }

    if (ClassToSpawn)
    {
        // Spawn the actor at the calculated world location
        GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnLocation, FRotator::ZeroRotator);

        // Fire VFX
        if (SpawnVFX)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnVFX, SpawnLocation);
        }

        // Fire SFX using Native Audio Component Instantiation
        if (SpawnSound)
        {
            UAudioComponent* AudioComp = NewObject<UAudioComponent>(GetWorld());
            if (AudioComp)
            {
                AudioComp->SetSound(SpawnSound);
                AudioComp->SetWorldLocation(SpawnLocation);
                AudioComp->bAutoDestroy = true; // Cleans up memory automatically
                AudioComp->RegisterComponentWithWorld(GetWorld());
                AudioComp->Play();
            }
        }
    }

    CurrentSpawnIndex++;

    // Clear the timer once we finish the array
    if (CurrentSpawnIndex >= CachedSpawnLocations.Num())
    {
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
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
            // Trace the perimeter of the triangle to prevent overlapping at the corners
            float PerimeterPct = (float)i / EnemyCount;

            if (PerimeterPct < 0.3333f) {
                // Right Edge: Top tip to Bottom Right
                float Progress = PerimeterPct * 3.0f;
                XOffset = FMath::Lerp(0.0f, FormationRadius, Progress);
                YOffset = FMath::Lerp(FormationRadius, -FormationRadius, Progress);
            }
            else if (PerimeterPct < 0.6666f) {
                // Bottom Edge: Bottom Right to Bottom Left
                float Progress = (PerimeterPct - 0.3333f) * 3.0f;
                XOffset = FMath::Lerp(FormationRadius, -FormationRadius, Progress);
                YOffset = -FormationRadius;
            }
            else {
                // Left Edge: Bottom Left back to Top tip
                float Progress = (PerimeterPct - 0.6666f) * 3.0f;
                XOffset = FMath::Lerp(-FormationRadius, 0.0f, Progress);
                YOffset = FMath::Lerp(-FormationRadius, FormationRadius, Progress);
            }
        }
        break;

        case EFormationShape::Line:
            // Spaced evenly along the X axis, centered on the trap plate
            XOffset = (i - ((EnemyCount - 1) / 2.0f)) * (FormationRadius * 0.75f);
            break;

        case EFormationShape::Cross:
        {
            if (i == 0)
            {
                // Anchor the first index exactly at the center
                XOffset = 0.0f;
                YOffset = 0.0f;
            }
            else {
                // Now divides the circle by the dynamic prong count
                int32 Arm = (i - 1) % CrossProngCount;
                int32 Depth = ((i - 1) / CrossProngCount) + 1;
                XOffset = FMath::Cos((TWO_PI / (float)CrossProngCount) * Arm) * (FormationRadius * 0.5f * Depth);
                YOffset = FMath::Sin((TWO_PI / (float)CrossProngCount) * Arm) * (FormationRadius * 0.5f * Depth);
            }
        }
        break;

        case EFormationShape::Pentagon:
        case EFormationShape::Hexagon:
        case EFormationShape::Octagon:
        {
            // Determine how many sides this polygon has
            int32 Sides = (Shape == EFormationShape::Pentagon) ? 5 : ((Shape == EFormationShape::Hexagon) ? 6 : 8);

            // Find exactly where this index sits along the total perimeter
            float PerimeterPct = (float)i / EnemyCount;
            int32 CurrentEdge = FMath::FloorToInt(PerimeterPct * Sides);
            float EdgeProgress = (PerimeterPct * Sides) - CurrentEdge;

            // Calculate the angles of the two vertices this edge connects (offset by -PI/2 to point "forward")
            float Angle1 = ((TWO_PI / Sides) * CurrentEdge) - (PI / 2.0f);
            float Angle2 = ((TWO_PI / Sides) * (CurrentEdge + 1)) - (PI / 2.0f);

            // Find the physical world coordinates of those two vertices
            FVector V1(FMath::Cos(Angle1) * FormationRadius, FMath::Sin(Angle1) * FormationRadius, 0.0f);
            FVector V2(FMath::Cos(Angle2) * FormationRadius, FMath::Sin(Angle2) * FormationRadius, 0.0f);

            // Lerp between them based on how far along the edge we are
            XOffset = FMath::Lerp(V1.X, V2.X, EdgeProgress);
            YOffset = FMath::Lerp(V1.Y, V2.Y, EdgeProgress);
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

        // Combine into a vector
        FVector LocalOffset(XOffset, YOffset, 0.0f);

        // Rotate the offset around the Z axis (UpVector) by the specified degrees
        FVector RotatedOffset = LocalOffset.RotateAngleAxis(FormationRotation, FVector::UpVector);

        Offsets.Add(RotatedOffset);
    }
    return Offsets;
}

#if WITH_EDITOR
void AFormationTrapPlate::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // Get the name of the variable the designer just changed
    FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

    // If they changed either the Count or the Shape, run the math check
    if (PropertyName == GET_MEMBER_NAME_CHECKED(AFormationTrapPlate, EnemyCount) ||
        PropertyName == GET_MEMBER_NAME_CHECKED(AFormationTrapPlate, SelectedShape) ||
        PropertyName == GET_MEMBER_NAME_CHECKED(AFormationTrapPlate, CrossProngCount))
    {
        EnforceShapeRules();
    }
}

void AFormationTrapPlate::EnforceShapeRules()
{
    if (EnemyCount < 1) EnemyCount = 1;

    switch (SelectedShape)
    {
    case EFormationShape::Square:
        // Snaps to nearest multiple of 4 (4, 8, 12, 16...)
        EnemyCount = FMath::Max(4, FMath::RoundToInt((float)EnemyCount / 4.0f) * 4);
        break;
    case EFormationShape::Cross:
        // Needs 1 center point, plus symmetrical arms
        // Adjusts itself to be dynamic, based on how many prongs it has.
		// For example, if CrossProngCount is 4, it will snap to 5, 9, 13, 17, etc.
		// Another example, if CrossProngCount is 7, it will snap to 8, 15, 22, 29, etc.
        EnemyCount = FMath::Max(CrossProngCount + 1, FMath::RoundToInt((float)(EnemyCount - 1) / CrossProngCount) * CrossProngCount + 1);
        break;
    case EFormationShape::Triangle:
        // Snaps to nearest multiple of 3 (3, 6, 9, 12...)
        EnemyCount = FMath::Max(3, FMath::RoundToInt((float)EnemyCount / 3.0f) * 3);
        break;
    case EFormationShape::Pentagon:
        // Snaps to nearest multiple of 5 (5, 10, 15, 20...)
        EnemyCount = FMath::Max(5, FMath::RoundToInt((float)EnemyCount / 5.0f) * 5);
        break;
    case EFormationShape::Hexagon:
        // Snaps to nearest multiple of 6 (6, 12, 18, 24...)
        EnemyCount = FMath::Max(6, FMath::RoundToInt((float)EnemyCount / 6.0f) * 6);
        break;
    case EFormationShape::Octagon:
		// Snaps to nearest multiple of 8 (8, 16, 24, 32...)
        EnemyCount = FMath::Max(8, FMath::RoundToInt((float)EnemyCount / 8.0f) * 8);
        break;
    case EFormationShape::TwoLines:
        // Snaps to even numbers (2, 4, 6, 8...)
        EnemyCount = FMath::Max(2, FMath::RoundToInt((float)EnemyCount / 2.0f) * 2);
        break;
    case EFormationShape::Wedge:
        // Forces odd numbers so the flying-V is always perfectly symmetrical (1, 3, 5, 7...)
        if (EnemyCount % 2 == 0) { EnemyCount += 1; }
        break;
    default:
        // Circle, Line, Arc, and Random have no structural restrictions
        break;
    }

    // Also ensure the LeaderIndex doesn't accidentally exceed the new restricted count
    if (LeaderIndex >= EnemyCount) { LeaderIndex = EnemyCount - 1; }
}
#endif
