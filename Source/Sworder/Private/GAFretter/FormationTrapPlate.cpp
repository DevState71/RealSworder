// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/FormationTrapPlate.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
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
    if (World && !HasActorBegunPlay() && Waves.Num() > 0)
    {
        // Transforms the draggable local offset into the correct world position
        FVector Center = GetTransform().TransformPosition(FormationCenterOffset);
        for (int32 WaveIdx = 0; WaveIdx < Waves.Num(); ++WaveIdx)
        {
            // Skip this wave if it's not the one we want to preview (and we aren't showing ALL waves with -1)
            if (PreviewWaveIndex != -1 && WaveIdx != PreviewWaveIndex)
            {
                continue;
            }

            FFormationWave PreviewWave = Waves[WaveIdx];

            if (PreviewWave.SelectedShape == EFormationShape::Random)
            {
                PreviewWave.SelectedShape = EFormationShape::Circle;
            }

            TArray<FVector> Offsets = CalculateFormationOffsets(PreviewWave);

            for (int32 i = 0; i < Offsets.Num(); ++i)
            {
                FVector SpawnLoc = Center + Offsets[i];
                // Highlight the Leader spawn position in Yellow
                FColor SphereColor = (i == PreviewWave.LeaderIndex && PreviewWave.LeaderClassToSpawn != nullptr) ? FColor::Yellow : FColor::Red;

                DrawDebugSphere(World, SpawnLoc, 30.0f, 12, SphereColor, false, -1.0f, 0, 2.0f);

                // Only draw the green connection lines for the first wave to prevent visual clutter
                if (WaveIdx == 0)
                {
                    DrawDebugLine(World, Center, SpawnLoc, FColor::Green, false, -1.0f, 0, 1.0f);
                }
            }
        }
    }
#endif
}

void AFormationTrapPlate::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Ensure we have a valid actor, a valid class to spawn, and at least 1 enemy requested
    if (OtherActor && OtherActor != this && Waves.Num() > 0)
    {
        // Disable the trigger so it only fires once per level
        TriggerZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        CurrentWaveIndex = 0; // Start at the first wave

        StartNextWave();
    }
}

void AFormationTrapPlate::StartNextWave()
{
    // Safety check: Are we out of waves?
    if (CurrentWaveIndex >= Waves.Num()) return;

    FFormationWave& CurrentWave = Waves[CurrentWaveIndex];
    FVector Center = GetTransform().TransformPosition(FormationCenterOffset);

    // Roll random shapes per wave if requested
    EFormationShape ShapeToSpawn = CurrentWave.SelectedShape;
    if (ShapeToSpawn == EFormationShape::Random)
    {
        uint8 MaxShapeIndex = static_cast<uint8>(EFormationShape::Random) - 1;
        CurrentWave.SelectedShape = static_cast<EFormationShape>(FMath::RandRange(0, MaxShapeIndex));
    }

    // Pass the entire wave struct so it has the right radius, count, etc.
    TArray<FVector> Offsets = CalculateFormationOffsets(CurrentWave);

    CachedSpawnLocations.Empty();
    for (const FVector& Offset : Offsets) { CachedSpawnLocations.Add(Center + Offset); }

    CurrentSpawnIndex = 0;

    // Kick off the staggered spawn loop for THIS wave
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AFormationTrapPlate::SpawnNextEnemy, CurrentWave.SpawnDelay, true, 0.0f);
}

void AFormationTrapPlate::SpawnNextEnemy()
{
    if (CurrentSpawnIndex >= CachedSpawnLocations.Num())
    {
        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
        return;
    }

    FFormationWave& CurrentWave = Waves[CurrentWaveIndex];
    FVector SpawnLocation = CachedSpawnLocations[CurrentSpawnIndex];

	// ===============================
    // NAVMESH VALIDATION
    // ===============================
    if (bProjectToNavMesh)
    {
        if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
        {
            FNavLocation ProjectedLocation;
            if (NavSys->ProjectPointToNavigation(SpawnLocation, ProjectedLocation, NavSearchExtent))
            {
                SpawnLocation = ProjectedLocation.Location;
            }
            else
            {
                // This is used as a fallback fro when the point is either in the void or in a wall.
                // Safely dumps the enemy at the center of the trap plate, so the game doesn't softlock.
                SpawnLocation = GetActorLocation();
            }
		}
    }

    // Determine if this index gets the Leader class or a Grunt
    TSubclassOf<AActor> ClassToSpawn = CurrentWave.EnemyClassToSpawn;
    if (CurrentSpawnIndex == CurrentWave.LeaderIndex && CurrentWave.LeaderClassToSpawn != nullptr)
    {
        ClassToSpawn = CurrentWave.LeaderClassToSpawn;
    }

    if (ClassToSpawn)
    {
        // Spawn the actor at the calculated world location
        GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnLocation, FRotator::ZeroRotator);

        // Fire VFX
        if (SpawnVFX) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnVFX, SpawnLocation);

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

        CurrentWaveIndex++; // Move to the next wave in the array

        // If there are more waves, queue up the next one using the designer's requested delay
        if (CurrentWaveIndex < Waves.Num())
        {
            GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AFormationTrapPlate::StartNextWave, CurrentWave.DelayBeforeNextWave, false);
        }
    }
}

TArray<FVector> AFormationTrapPlate::CalculateFormationOffsets(const FFormationWave& Wave) const
{
    TArray<FVector> Offsets;
    if (Wave.EnemyCount <= 0) return Offsets;

    for (int32 i = 0; i < Wave.EnemyCount; ++i)
    {
        // Angle step size based on total enemy count for circular shapes
        float Angle = (TWO_PI / (float)Wave.EnemyCount) * i;
        float XOffset = 0.0f;
        float YOffset = 0.0f;

        switch (Wave.SelectedShape)
        {
        case EFormationShape::Circle:
            XOffset = FMath::Cos(Angle) * Wave.FormationRadius;
            YOffset = FMath::Sin(Angle) * Wave.FormationRadius;
            break;

        case EFormationShape::Square:
        {
            // Dynamic square: calculates perimeter position based on percentage
            float PerimeterPct = (float)i / Wave.EnemyCount;
            if (PerimeterPct < 0.25f) { // Top edge
                XOffset = Wave.FormationRadius;
                YOffset = FMath::Lerp(-Wave.FormationRadius, Wave.FormationRadius, PerimeterPct * 4.0f);
            }
            else if (PerimeterPct < 0.5f) { // Right edge
                XOffset = FMath::Lerp(Wave.FormationRadius, -Wave.FormationRadius, (PerimeterPct - 0.25f) * 4.0f);
                YOffset = Wave.FormationRadius;
            }
            else if (PerimeterPct < 0.75f) { // Bottom edge
                XOffset = -Wave.FormationRadius;
                YOffset = FMath::Lerp(Wave.FormationRadius, -Wave.FormationRadius, (PerimeterPct - 0.5f) * 4.0f);
            }
            else { // Left edge
                XOffset = FMath::Lerp(-Wave.FormationRadius, Wave.FormationRadius, (PerimeterPct - 0.75f) * 4.0f);
                YOffset = -Wave.FormationRadius;
            }
        }
        break;

        case EFormationShape::Triangle:
        {
            // Trace the perimeter of the triangle to prevent overlapping at the corners
            float PerimeterPct = (float)i / Wave.EnemyCount;
            if (PerimeterPct < 0.3333f) {
                float Progress = PerimeterPct * 3.0f;
                XOffset = FMath::Lerp(0.0f, Wave.FormationRadius, Progress);
                YOffset = FMath::Lerp(Wave.FormationRadius, -Wave.FormationRadius, Progress);
            }
            else if (PerimeterPct < 0.6666f) {
                float Progress = (PerimeterPct - 0.3333f) * 3.0f;
                XOffset = FMath::Lerp(Wave.FormationRadius, -Wave.FormationRadius, Progress);
                YOffset = -Wave.FormationRadius;
            }
            else {
                float Progress = (PerimeterPct - 0.6666f) * 3.0f;
                XOffset = FMath::Lerp(-Wave.FormationRadius, 0.0f, Progress);
                YOffset = FMath::Lerp(-Wave.FormationRadius, Wave.FormationRadius, Progress);
            }
        }
        break;

        case EFormationShape::Line:
            // Spaced evenly along the X axis, centered on the trap plate
            XOffset = (i - ((Wave.EnemyCount - 1) / 2.0f)) * (Wave.FormationRadius * 0.75f);
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
                int32 Arm = (i - 1) % Wave.CrossProngCount;
                int32 Depth = ((i - 1) / Wave.CrossProngCount) + 1;
                XOffset = FMath::Cos((TWO_PI / (float)Wave.CrossProngCount) * Arm) * (Wave.FormationRadius * 0.5f * Depth);
                YOffset = FMath::Sin((TWO_PI / (float)Wave.CrossProngCount) * Arm) * (Wave.FormationRadius * 0.5f * Depth);
            }
        }
        break;

        case EFormationShape::Pentagon:
        case EFormationShape::Hexagon:
        case EFormationShape::Octagon:
        {
            int32 Sides = (Wave.SelectedShape == EFormationShape::Pentagon) ? 5 : ((Wave.SelectedShape == EFormationShape::Hexagon) ? 6 : 8);
            float PerimeterPct = (float)i / Wave.EnemyCount;
            int32 CurrentEdge = FMath::FloorToInt(PerimeterPct * Sides);
            float EdgeProgress = (PerimeterPct * Sides) - CurrentEdge;

            float Angle1 = ((TWO_PI / Sides) * CurrentEdge) - (PI / 2.0f);
            float Angle2 = ((TWO_PI / Sides) * (CurrentEdge + 1)) - (PI / 2.0f);

            FVector V1(FMath::Cos(Angle1) * Wave.FormationRadius, FMath::Sin(Angle1) * Wave.FormationRadius, 0.0f);
            FVector V2(FMath::Cos(Angle2) * Wave.FormationRadius, FMath::Sin(Angle2) * Wave.FormationRadius, 0.0f);

            XOffset = FMath::Lerp(V1.X, V2.X, EdgeProgress);
            YOffset = FMath::Lerp(V1.Y, V2.Y, EdgeProgress);
        }
        break;

        case EFormationShape::TwoLines:
        {
            int32 HalfCount = FMath::CeilToInt((float)Wave.EnemyCount / 2.0f);
            bool bTopRow = (i < HalfCount);
            int32 RowIndex = bTopRow ? i : (i - HalfCount);
            XOffset = (RowIndex - ((HalfCount - 1) / 2.0f)) * (Wave.FormationRadius * 0.75f);
            YOffset = bTopRow ? (Wave.FormationRadius * 0.5f) : -(Wave.FormationRadius * 0.5f);
        }
        break;

        case EFormationShape::Arc:
        {
            // Spaces the enemies across a 180-degree semi-circle
            float ArcAngle = (PI / FMath::Max(1.0f, (float)Wave.EnemyCount - 1.0f)) * i;
            XOffset = FMath::Cos(ArcAngle) * Wave.FormationRadius;
            YOffset = FMath::Sin(ArcAngle) * Wave.FormationRadius;
        }
        break;

        case EFormationShape::Wedge:
            if (i == 0) {
                XOffset = 0.0f;
                YOffset = Wave.FormationRadius;
            }
            else {
                // One leader at the front, alternating followers forming the V wings
                int32 Row = (i + 1) / 2;
                float Side = (i % 2 == 0) ? 1.0f : -1.0f;
                XOffset = Side * Row * (Wave.FormationRadius * 0.4f);
                YOffset = Wave.FormationRadius - (Row * (Wave.FormationRadius * 0.5f));
            }
            break;

        case EFormationShape::Random:
            // Fallback to prevent compiler warnings, though logic prevents reaching here
            break;
        }

        // Combine into a vector
        FVector LocalOffset(XOffset, YOffset, 0.0f);

        // Rotate the offset around the Z axis (UpVector) by the specified degrees
        FVector RotatedOffset = LocalOffset.RotateAngleAxis(Wave.FormationRotation, FVector::UpVector);

        Offsets.Add(RotatedOffset);
    }
    return Offsets;
}

#if WITH_EDITOR
void AFormationTrapPlate::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // Get the name of the variable the designer just changed
    FName MemberPropertyName = (PropertyChangedEvent.MemberProperty != nullptr) ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;

    // If they changed either the Count or the Shape, run the math check
    if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(AFormationTrapPlate, Waves))
    {
        EnforceShapeRules();
    }
}

void AFormationTrapPlate::EnforceShapeRules()
{
    for (FFormationWave& Wave : Waves)
    {
        if (Wave.EnemyCount < 1) Wave.EnemyCount = 1;

        switch (Wave.SelectedShape)
        {
        case EFormationShape::Square:
            // Snaps to nearest multiple of 4 (4, 8, 12, 16...)
            Wave.EnemyCount = FMath::Max(4, FMath::RoundToInt((float)Wave.EnemyCount / 4.0f) * 4);
            break;
        case EFormationShape::Cross:
            // Needs 1 center point, plus symmetrical arms
            // Adjusts itself to be dynamic, based on how many prongs it has.
            // For example, if CrossProngCount is 4, it will snap to 5, 9, 13, 17, etc.
            // Another example, if CrossProngCount is 7, it will snap to 8, 15, 22, 29, etc.
            Wave.EnemyCount = FMath::Max(Wave.CrossProngCount + 1, FMath::RoundToInt((float)(Wave.EnemyCount - 1) / Wave.CrossProngCount) * Wave.CrossProngCount + 1);
            break;
        case EFormationShape::Triangle:
            // Snaps to nearest multiple of 3 (3, 6, 9, 12...)
            Wave.EnemyCount = FMath::Max(3, FMath::RoundToInt((float)Wave.EnemyCount / 3.0f) * 3);
            break;
        case EFormationShape::Pentagon:
            // Snaps to nearest multiple of 5 (5, 10, 15, 20...)
            Wave.EnemyCount = FMath::Max(5, FMath::RoundToInt((float)Wave.EnemyCount / 5.0f) * 5);
            break;
        case EFormationShape::Hexagon:
            // Snaps to nearest multiple of 6 (6, 12, 18, 24...)
            Wave.EnemyCount = FMath::Max(6, FMath::RoundToInt((float)Wave.EnemyCount / 6.0f) * 6);
            break;
        case EFormationShape::Octagon:
            // Snaps to nearest multiple of 8 (8, 16, 24, 32...)
            Wave.EnemyCount = FMath::Max(8, FMath::RoundToInt((float)Wave.EnemyCount / 8.0f) * 8);
            break;
        case EFormationShape::TwoLines:
            // Snaps to even numbers (2, 4, 6, 8...)
            Wave.EnemyCount = FMath::Max(2, FMath::RoundToInt((float)Wave.EnemyCount / 2.0f) * 2);
            break;
        case EFormationShape::Wedge:
            // Forces odd numbers so the flying-V is always perfectly symmetrical (1, 3, 5, 7...)
            if (Wave.EnemyCount % 2 == 0) { Wave.EnemyCount += 1; }
            break;
        default:
            // Circle, Line, Arc, and Random have no structural restrictions
            break;
        }

        // Also ensure the LeaderIndex doesn't accidentally exceed the new restricted count
        if (Wave.LeaderIndex >= Wave.EnemyCount) { Wave.LeaderIndex = Wave.EnemyCount - 1; }
    }

    // Ensure the preview index doesn't exceed the number of waves we actually have
    if (PreviewWaveIndex >= Waves.Num())
    {
        PreviewWaveIndex = Waves.Num() - 1;
    }
}
#endif
