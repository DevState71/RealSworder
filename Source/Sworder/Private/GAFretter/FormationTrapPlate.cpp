// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/FormationTrapPlate.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
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

    // Spline Component
    AmbushSpline = CreateDefaultSubobject<USplineComponent>(TEXT("AmbushSpline"));
    AmbushSpline->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFormationTrapPlate::BeginPlay()
{
	Super::BeginPlay();
	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AFormationTrapPlate::OnOverlap);
    InitializeObjectPool();
}

void AFormationTrapPlate::InitializeObjectPool()
{
    // Tally up exactly how many of each enemy class we need across all waves combined
    TMap<TSubclassOf<AActor>, int32> RequiredEnemies;

    for (const FFormationWave& Wave : Waves)
    {
        if (Wave.EnemyCount <= 0) continue;

        // Tally Grunts
        if (Wave.EnemyClassToSpawn)
        {
            int32 GruntCount = Wave.EnemyCount;
            if (Wave.LeaderClassToSpawn != nullptr) { GruntCount -= 1; }

            RequiredEnemies.FindOrAdd(Wave.EnemyClassToSpawn) += GruntCount;
        }

        // Tally Leaders
        if (Wave.LeaderClassToSpawn)
        {
            RequiredEnemies.FindOrAdd(Wave.LeaderClassToSpawn) += 1;
        }
    }

    // Spawn them all hidden underground and disable their logic
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    for (const auto& KVP : RequiredEnemies)
    {
        TSubclassOf<AActor> ClassToSpawn = KVP.Key;
        int32 AmountToSpawn = KVP.Value;

        for (int32 i = 0; i < AmountToSpawn; ++i)
        {
            // Spawn them at -10000 on the Z-Axis, so they're safely out of the playable area
            if (AActor* NewEnemy = GetWorld()->SpawnActor<AActor>(ClassToSpawn, FVector(0.0f, 0.0f, -10000.0f), FRotator::ZeroRotator, SpawnParams))
            {
                NewEnemy->SetActorHiddenInGame(true);
                NewEnemy->SetActorEnableCollision(false);
                NewEnemy->SetActorTickEnabled(false);
                PooledEnemies.Add(NewEnemy);
            }

        }
    }
}

AActor* AFormationTrapPlate::GetEnemyFromPool(TSubclassOf<AActor> EnemyClass)
{
    // Find the 1st matching enemy that's currently sleeping
    for (AActor* Enemy : PooledEnemies)
    {
        if (Enemy && Enemy->IsA(EnemyClass) && Enemy->IsHidden())
        {
            return Enemy;
        }
    }

    // Fallback: if we ran out, spawn a new one on the fly
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AActor* EmergencySpawn = GetWorld()->SpawnActor<AActor>(EnemyClass, FVector(0.0f, 0.0f, -10000.0f), FRotator::ZeroRotator, SpawnParams);

    if (EmergencySpawn) { PooledEnemies.Add(EmergencySpawn); }

    return EmergencySpawn;
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

        // Distinct color palette to separate overlapping waves
        TArray<FColor> WaveColors = { FColor::Red, FColor::Cyan, FColor::Magenta, FColor::Orange, FColor::Blue, FColor::Emerald, FColor::Purple };

        for (int32 WaveIdx = 0; WaveIdx < Waves.Num(); ++WaveIdx)
        {
            // Skip this wave if it's not the one we want to preview (and we aren't showing ALL waves with -1)
            if (PreviewWaveIndex != -1 && WaveIdx != PreviewWaveIndex) { continue; }

            FFormationWave PreviewWave = Waves[WaveIdx];
            if (PreviewWave.SelectedShape == EFormationShape::Random) { PreviewWave.SelectedShape = EFormationShape::Circle; }
            TArray<FVector> Offsets = CalculateFormationOffsets(PreviewWave);

            // Pre-calculate the custom facing point in world space for this specific wave
            FVector WorldCustomFacingPoint = GetTransform().TransformPosition(PreviewWave.CustomFacingPoint);

            // Fetch the assigned color for this specific wave index
            FColor CurrentWaveColor = WaveColors[WaveIdx % WaveColors.Num()];

            for (int32 i = 0; i < Offsets.Num(); ++i)
            {
                FVector SpawnLoc = Center + Offsets[i];
                // Highlight the Leader spawn position in Yellow
                FColor SphereColor = (i == PreviewWave.LeaderIndex && PreviewWave.LeaderClassToSpawn != nullptr) ? FColor::Yellow : CurrentWaveColor;

                DrawDebugSphere(World, SpawnLoc, 30.0f, 12, SphereColor, false, -1.0f, 0, 2.0f);

                // ==========================================
                // EDITOR VISUALIZATION: FACING DIRECTION
				// ------------------------------------------
                if (PreviewWave.bFacePlayer)
                {
                    // "Player Seeking" Symbol
                    // Draw an arrow that points directly radially outward from the center to symbolize scanning for the player
                    FVector RadiaDir = (SpawnLoc - Center).GetSafeNormal();

                    // Fallback if the enemy is spawninfg exactly dead-center on the plate
                    if (RadiaDir.IsNearlyZero()) { RadiaDir = GetActorForwardVector(); }

                    FVector ArrowEnd = SpawnLoc + (RadiaDir * 75.0f);
                    DrawDebugDirectionalArrow(World, SpawnLoc, ArrowEnd, 30.0f, CurrentWaveColor, false, -1.0f, 0, 3.0f);
                }
                else
                {
                    // "Custom Target" Symbol
                    // Draws an arrow pointing exactly at the CustomFacingPoint
                    FVector TargetDir = (WorldCustomFacingPoint - SpawnLoc);
                    TargetDir.Z = 0.0f; // Flattened like the runtimespawn logic

                    if (!TargetDir.IsNearlyZero())
                    {
                        TargetDir.Normalize();
                        FVector ArrowEnd = SpawnLoc + (TargetDir * 90.0f);
                        DrawDebugDirectionalArrow(World, SpawnLoc, ArrowEnd, 30.0f, CurrentWaveColor, false, -1.0f, 0, 3.0f);
                    }

                    // Render a visible box at the custom target point so the designer can locate it easily
                    if (i == 0) // Only draw this once per wave to avoid rendering overlapping boxes
                    {
                        DrawDebugBox(World, WorldCustomFacingPoint, FVector(15.0f), CurrentWaveColor, false, -1.0f, 0, 3.0f);
                    }
                }


				// ==========================================

                // Only draw the green connection lines for the first wave to prevent visual clutter
                if (WaveIdx == 0) { DrawDebugLine(World, Center, SpawnLoc, CurrentWaveColor, false, -1.0f, 0, 1.0f); }
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
        // ================================================
        // ROTATION CALCULATION
        // ------------------------------------------------
		FRotator SpawnRotation = FRotator::ZeroRotator;
        FVector TargetLocation;
        bool bFoundTarget = false;

        if (CurrentWave.bFacePlayer)
        {
            // Safely fetch the active player pawn
            if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
            {
                if (APawn* PlayerPawn = PC->GetPawn())
                {
                    TargetLocation = PlayerPawn->GetActorLocation();
					bFoundTarget = true;
                }
            }
        }
        else
        {
            TargetLocation = GetTransform().TransformPosition(CurrentWave.CustomFacingPoint);
            bFoundTarget = true;
        }

        if (bFoundTarget)
        {
            FVector DirectionToTarget = TargetLocation - SpawnLocation;
            DirectionToTarget.Z = 0.0f;

			// Only apply rotation if enemy is NOT spawning exactly ON the target point
            if (!DirectionToTarget.IsNearlyZero())
            {
                SpawnRotation = DirectionToTarget.Rotation();
			}
        }

        // ================================================
        // PRE-SPAWN TELEGRAPHING
        // ------------------------------------------------
        // Spawn warning telegraph immediately at the NavMesh Location
        if (TelegraphVFX)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TelegraphVFX,SpawnLocation, SpawnRotation);
        }

        // Package Spawn data into delegate, allowing the timer to remember what to spawn and where
        FTimerDelegate SpawnDelegate;
        SpawnDelegate.BindUObject(this, &AFormationTrapPlate::ExecuteSpawn, ClassToSpawn, SpawnLocation, SpawnRotation);

        // Fire actual spawn after the designer's requested telegraph delay
        FTimerHandle TempTelegraphHandle;
        if (CurrentWave.TelegraphDelay > 0.0f)
        {
            GetWorldTimerManager().SetTimer(TempTelegraphHandle, SpawnDelegate, CurrentWave.TelegraphDelay, false);
        }
        else
        {
            // If delay is set to 0, spawn it instantly
            ExecuteSpawn(ClassToSpawn, SpawnLocation, SpawnRotation);
        }
        // ================================================
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
        float XOffset = 0.0f; float YOffset = 0.0f;

        switch (Wave.SelectedShape)
        {
        case EFormationShape::Circle: XOffset = FMath::Cos(Angle) * Wave.FormationRadius; YOffset = FMath::Sin(Angle) * Wave.FormationRadius; break;

        case EFormationShape::Square:
        {
            // Dynamic square: calculates perimeter position based on percentage
            float PerimeterPct = (float)i / Wave.EnemyCount;
            // Top edge, right edge, bottom edge, and left edge, respectively
            if (PerimeterPct < 0.25f) { XOffset = Wave.FormationRadius; YOffset = FMath::Lerp(-Wave.FormationRadius, Wave.FormationRadius, PerimeterPct * 4.0f); }
            else if (PerimeterPct < 0.5f) { XOffset = FMath::Lerp(Wave.FormationRadius, -Wave.FormationRadius, (PerimeterPct - 0.25f) * 4.0f); YOffset = Wave.FormationRadius; }
            else if (PerimeterPct < 0.75f) { XOffset = -Wave.FormationRadius; YOffset = FMath::Lerp(Wave.FormationRadius, -Wave.FormationRadius, (PerimeterPct - 0.5f) * 4.0f); }
            else { XOffset = FMath::Lerp(-Wave.FormationRadius, Wave.FormationRadius, (PerimeterPct - 0.75f) * 4.0f); YOffset = -Wave.FormationRadius; }
        } break;

        case EFormationShape::Triangle:
        {
            // Trace the perimeter of the triangle to prevent overlapping at the corners
            float PerimeterPct = (float)i / Wave.EnemyCount;
            if (PerimeterPct < 0.3333f) { float Progress = PerimeterPct * 3.0f; XOffset = FMath::Lerp(0.0f, Wave.FormationRadius, Progress); YOffset = FMath::Lerp(Wave.FormationRadius, -Wave.FormationRadius, Progress); }
            else if (PerimeterPct < 0.6666f) { float Progress = (PerimeterPct - 0.3333f) * 3.0f; XOffset = FMath::Lerp(Wave.FormationRadius, -Wave.FormationRadius, Progress); YOffset = -Wave.FormationRadius; }
            else { float Progress = (PerimeterPct - 0.6666f) * 3.0f; XOffset = FMath::Lerp(-Wave.FormationRadius, 0.0f, Progress); YOffset = FMath::Lerp(-Wave.FormationRadius, Wave.FormationRadius, Progress); }
        } break;

        // Spaced evenly along the X axis, centered on the trap plate
        case EFormationShape::Line: XOffset = (i - ((Wave.EnemyCount - 1) / 2.0f)) * (Wave.FormationRadius * 0.75f); break;

        case EFormationShape::Cross:
        {
            // Anchor the first index exactly at the center
            if (i == 0) { XOffset = 0.0f; YOffset = 0.0f; }
            else {
                int32 Arm = (i - 1) % Wave.CrossProngCount; int32 Depth = ((i - 1) / Wave.CrossProngCount) + 1;
                XOffset = FMath::Cos((TWO_PI / (float)Wave.CrossProngCount) * Arm) * (Wave.FormationRadius * 0.5f * Depth); YOffset = FMath::Sin((TWO_PI / (float)Wave.CrossProngCount) * Arm) * (Wave.FormationRadius * 0.5f * Depth);
            }
        } break;

        case EFormationShape::Pentagon: case EFormationShape::Hexagon: case EFormationShape::Octagon:
        {
            int32 Sides = (Wave.SelectedShape == EFormationShape::Pentagon) ? 5 : ((Wave.SelectedShape == EFormationShape::Hexagon) ? 6 : 8);
            float PerimeterPct = (float)i / Wave.EnemyCount; int32 CurrentEdge = FMath::FloorToInt(PerimeterPct * Sides); float EdgeProgress = (PerimeterPct * Sides) - CurrentEdge;
            float Angle1 = ((TWO_PI / Sides) * CurrentEdge) - (PI / 2.0f); float Angle2 = ((TWO_PI / Sides) * (CurrentEdge + 1)) - (PI / 2.0f);
            FVector V1(FMath::Cos(Angle1) * Wave.FormationRadius, FMath::Sin(Angle1) * Wave.FormationRadius, 0.0f); FVector V2(FMath::Cos(Angle2) * Wave.FormationRadius, FMath::Sin(Angle2) * Wave.FormationRadius, 0.0f);
            XOffset = FMath::Lerp(V1.X, V2.X, EdgeProgress); YOffset = FMath::Lerp(V1.Y, V2.Y, EdgeProgress);
        } break;

        case EFormationShape::TwoLines:
        {
            int32 HalfCount = FMath::CeilToInt((float)Wave.EnemyCount / 2.0f); bool bTopRow = (i < HalfCount); int32 RowIndex = bTopRow ? i : (i - HalfCount);
            XOffset = (RowIndex - ((HalfCount - 1) / 2.0f)) * (Wave.FormationRadius * 0.75f); YOffset = bTopRow ? (Wave.FormationRadius * 0.5f) : -(Wave.FormationRadius * 0.5f);
        } break;

        // Spaces the enemies across a 180-degree semi-circle
        case EFormationShape::Arc: { float ArcAngle = (PI / FMath::Max(1.0f, (float)Wave.EnemyCount - 1.0f)) * i; XOffset = FMath::Cos(ArcAngle) * Wave.FormationRadius; YOffset = FMath::Sin(ArcAngle) * Wave.FormationRadius; } break;

        case EFormationShape::Wedge:
            if (i == 0) { XOffset = 0.0f; YOffset = Wave.FormationRadius; }
            else {
                // One leader at the front, alternating followers forming the V wings
                int32 Row = (i + 1) / 2; float Side = (i % 2 == 0) ? 1.0f : -1.0f;
                XOffset = Side * Row * (Wave.FormationRadius * 0.4f); YOffset = Wave.FormationRadius - (Row * (Wave.FormationRadius * 0.5f));
            } break;

        case EFormationShape::Spline:
        {
            if (AmbushSpline)
            {
                // Get total length of the drawn path
                float SplineLength = AmbushSpline->GetSplineLength();

                // Space enemies perfectly evenly across the entire line
                float DistanceStep = (Wave.EnemyCount > 1) ? (SplineLength / (Wave.EnemyCount - 1)) : 0.0f;
                float Distance = (Wave.EnemyCount == 1) ? (SplineLength * 0.5f) : (i * DistanceStep);

                // Find exactly where the distance falls in world space
                FVector SplineWorldLoc = AmbushSpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

                // Convert to relative offset, playing it nicelt with existing spawner logic
                FVector Center = GetTransform().TransformPosition(FormationCenterOffset);
                Offsets.Add(SplineWorldLoc - Center);

                // This skips the standard LocalOffset rotation math at the bottom of loop
                continue;
            }
        } break;

        // Fallback to prevent compiler warnings, though logic prevents reaching here
        case EFormationShape::Random: break;
        }

        // Combine into a vector
        FVector LocalOffset(XOffset, YOffset, 0.0f);

        // Rotate the offset around the Z axis (UpVector) by the specified degrees
        FVector RotatedOffset = LocalOffset.RotateAngleAxis(Wave.FormationRotation, FVector::UpVector);

        Offsets.Add(RotatedOffset);
    }
    return Offsets;
}

void AFormationTrapPlate::ExecuteSpawn(TSubclassOf<AActor> ClassToSpawn, FVector SpawnLocation, FRotator SpawnRotation)
{
    if (ClassToSpawn)
    {
        // Pull a pre-allocated enemy from memory instead of spawning a new one
        if (AActor* PooledEnemy = GetEnemyFromPool(ClassToSpawn))
        {
            // Teleport it to the NavMesh Location
            PooledEnemy->SetActorLocationAndRotation(SpawnLocation, SpawnRotation, false, nullptr, ETeleportType::TeleportPhysics);

            // Wake up the enemy
            PooledEnemy->SetActorHiddenInGame(false);
            PooledEnemy->SetActorEnableCollision(true);
            PooledEnemy->SetActorTickEnabled(true);

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
    }
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
        case EFormationShape::Square: // Snaps to nearest multiple of 4 (4, 8, 12, 16...)
            Wave.EnemyCount = FMath::Max(4, FMath::RoundToInt((float)Wave.EnemyCount / 4.0f) * 4); break;
        case EFormationShape::Cross:
            // Needs 1 center point, plus symmetrical arms; Adjusts itself to be dynamic, based on how many prongs it has.
            // Examples, if CrossProngCount is 4, it will snap to 5, 9, 13, 17, etc.; if CrossProngCount is 7, it will snap to 8, 15, 22, 29, etc.
            Wave.EnemyCount = FMath::Max(Wave.CrossProngCount + 1, FMath::RoundToInt((float)(Wave.EnemyCount - 1) / Wave.CrossProngCount) * Wave.CrossProngCount + 1); break;
        case EFormationShape::Triangle: // Snaps to nearest multiple of 3 (3, 6, 9, 12...)
            Wave.EnemyCount = FMath::Max(3, FMath::RoundToInt((float)Wave.EnemyCount / 3.0f) * 3); break;
        case EFormationShape::Pentagon: // Snaps to nearest multiple of 5 (5, 10, 15, 20...)
            Wave.EnemyCount = FMath::Max(5, FMath::RoundToInt((float)Wave.EnemyCount / 5.0f) * 5); break;
        case EFormationShape::Hexagon: // Snaps to nearest multiple of 6 (6, 12, 18, 24...)
            Wave.EnemyCount = FMath::Max(6, FMath::RoundToInt((float)Wave.EnemyCount / 6.0f) * 6); break;
        case EFormationShape::Octagon: // Snaps to nearest multiple of 8 (8, 16, 24, 32...)
            Wave.EnemyCount = FMath::Max(8, FMath::RoundToInt((float)Wave.EnemyCount / 8.0f) * 8); break;
        case EFormationShape::TwoLines: // Snaps to even numbers (2, 4, 6, 8...)
            Wave.EnemyCount = FMath::Max(2, FMath::RoundToInt((float)Wave.EnemyCount / 2.0f) * 2); break;
        case EFormationShape::Wedge: // Forces odd numbers so the flying-V is always perfectly symmetrical (1, 3, 5, 7...)
            if (Wave.EnemyCount % 2 == 0) { Wave.EnemyCount += 1; } break;
        default: // Circle, Line, Arc, and Random have no structural restrictions
            break;
        }

        // Also ensure the LeaderIndex doesn't accidentally exceed the new restricted count
        if (Wave.LeaderIndex >= Wave.EnemyCount) { Wave.LeaderIndex = Wave.EnemyCount - 1; }
    }

    // Ensure the preview index doesn't exceed the number of waves we actually have
    if (PreviewWaveIndex >= Waves.Num()) { PreviewWaveIndex = Waves.Num() - 1; }
}
#endif
