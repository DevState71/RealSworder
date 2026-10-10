// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/BossPortalDirector.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraActor.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ABossPortalDirector::ABossPortalDirector()
{
 	// No need for ticks.
	PrimaryActorTick.bCanEverTick = true;

    RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = RootComp;

    ArenaTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ArenaTrigger"));
    ArenaTrigger->SetupAttachment(RootComponent);
    ArenaTrigger->SetBoxExtent(FVector(500.0f, 500.0f, 200.0f)); // Default Size
    ArenaTrigger->SetCollisionProfileName(TEXT("Trigger"));
    ArenaTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABossPortalDirector::OnPlayerEnterArena);

}

bool ABossPortalDirector::ShouldTickIfViewportsOnly() const { return true; }

// ==========================================
// ARENA LOCKDOWN
// ==========================================
void ABossPortalDirector::OnPlayerEnterArena(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // If a player walks in, lock the doors
    if (OtherActor && OtherActor->IsA(APawn::StaticClass()))
    {
        for (AActor* Door : LinkedArenaDoors)
        {
            if (Door)
            {
                // Lock blockout doors by making them visible and enabling collision
                Door->SetActorHiddenInGame(false);
                Door->SetActorEnableCollision(true);
            }
        }
        // Turn off the trigger, so it doesn't fire again
        ArenaTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

// ==========================================
// REUSABLE SHAPE MATH
// ==========================================
FVector ABossPortalDirector::CalculateShapeOffset(float Pct, float Radius) const
{
    float XUnit = 0.0f;
    float YUnit = 0.0f;

    switch (SpawnShape)
    {
        case EPortalSpawnShape::Circle:
        {
            float Angle = Pct * TWO_PI;
            XUnit = FMath::Cos(Angle); YUnit = FMath::Sin(Angle);
            break;
        }
        case EPortalSpawnShape::Square:
        {
            if (Pct < 0.25f) { XUnit = 1.0f; YUnit = FMath::Lerp(-1.0f, 1.0f, Pct * 4.0f); }
            else if (Pct < 0.5f) { XUnit = FMath::Lerp(1.0f, -1.0f, (Pct - 0.25f) * 4.0f); YUnit = 1.0f; }
            else if (Pct < 0.75f) { XUnit = -1.0f; YUnit = FMath::Lerp(1.0f, -1.0f, (Pct - 0.5f) * 4.0f); }
            else { XUnit = FMath::Lerp(-1.0f, 1.0f, (Pct - 0.75f) * 4.0f); YUnit = -1.0f; }
            break;
        }
        case EPortalSpawnShape::Triangle:
        {
            if (Pct < 0.3333f) { float P = Pct * 3.0f; XUnit = FMath::Lerp(0.0f, 1.0f, P); YUnit = FMath::Lerp(1.0f, -1.0f, P); }
            else if (Pct < 0.6666f) { float P = (Pct - 0.3333f) * 3.0f; XUnit = FMath::Lerp(1.0f, -1.0f, P); YUnit = -1.0f; }
            else { float P = (Pct - 0.6666f) * 3.0f; XUnit = FMath::Lerp(-1.0f, 0.0f, P); YUnit = FMath::Lerp(-1.0f, 1.0f, P); }
            break;
        }
        case EPortalSpawnShape::Hexagon:
        case EPortalSpawnShape::Octagon:
        {
            int32 Sides = (SpawnShape == EPortalSpawnShape::Hexagon) ? 6 : 8;
            int32 CurrentEdge = FMath::FloorToInt(Pct * Sides);
            float EdgeProgress = (Pct * Sides) - CurrentEdge;
            float Angle1 = ((TWO_PI / Sides) * CurrentEdge) - (PI / 2.0f);
            float Angle2 = ((TWO_PI / Sides) * (CurrentEdge + 1)) - (PI / 2.0f);

            FVector V1(FMath::Cos(Angle1), FMath::Sin(Angle1), 0.0f);
            FVector V2(FMath::Cos(Angle2), FMath::Sin(Angle2), 0.0f);

            XUnit = FMath::Lerp(V1.X, V2.X, EdgeProgress);
            YUnit = FMath::Lerp(V1.Y, V2.Y, EdgeProgress);
            break;
        }
    }

    return FVector(XUnit * Radius, YUnit * Radius, 0.0f);
}

// ==========================================
// EDITOR VISUALIZATION
// ==========================================
void ABossPortalDirector::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

#if WITH_EDITOR
    UWorld* World = GetWorld();
    if (World && !HasActorBegunPlay())
    {
        FVector Center = GetTransform().TransformPosition(PortalCenterOffset);

        if (bUseManualAnchors)
        {
            for (int32 i = 0; i < ManualPortalAnchors.Num(); ++i)
            {
                // Since the offsets are local, it's converted to World Space to draw them properly
                FVector WorldAnchor = GetTransform().TransformPosition(ManualPortalAnchors[i]);

                // Draw a Cyan box at the designer's chosen anchor point
                DrawDebugSolidBox(World, WorldAnchor, FVector(15.0f), FColor::Cyan, false, -1.0f, 0);

                // Draw a connection line back to the Director Anchor
                DrawDebugLine(World, Center, WorldAnchor, FColor::Cyan, false, -1.0f, 0, 1.5f);

                // Hover the array index number slightly above the diamond
                DrawDebugString(World, WorldAnchor + FVector(0.0f, 0.0f, 40.0f), FString::Printf(TEXT("Anchor [%d]"), i), nullptr, FColor::White, 0.0f, true);
            }
        }
        else
        {
            // Determine how many lines to draw based on the shape
            int32 Resolution = 32; // Default for Circle to make it smooth
            switch (SpawnShape)
            {
                case EPortalSpawnShape::Square: Resolution = 4; break;
                case EPortalSpawnShape::Triangle: Resolution = 3; break;
                case EPortalSpawnShape::Hexagon: Resolution = 6; break;
                case EPortalSpawnShape::Octagon: Resolution = 8; break;
            }

            for (int32 i = 0; i < Resolution; ++i)
            {
                float Pct1 = (float)i / Resolution;
                float Pct2 = (float)(i + 1) / Resolution;

                // Draw Red Inner Boundary (No-Spawn Zone)
                FVector MinP1 = Center + CalculateShapeOffset(Pct1, MinSpawnRadius);
                FVector MinP2 = Center + CalculateShapeOffset(Pct2, MinSpawnRadius);
                DrawDebugLine(World, MinP1, MinP2, FColor::Red, false, -1.0f, 0, 3.0f);

                // Draw Green Outer Boundary (Maximum Spawn Range)
                FVector MaxP1 = Center + CalculateShapeOffset(Pct1, MaxSpawnRadius);
                FVector MaxP2 = Center + CalculateShapeOffset(Pct2, MaxSpawnRadius);
                DrawDebugLine(World, MaxP1, MaxP2, FColor::Green, false, -1.0f, 0, 3.0f);
            }
        }
    }
#endif
}

// ==========================================
// CINEMATIC SEQUENCE STEP 1: TRIGGER
// ==========================================
void ABossPortalDirector::SpawnExitPortal(FVector BossDeathLocation)
{
    if (!PortalClassToSpawn) return;

    UWorld* World = GetWorld();
    if (!World) return;

    FVector PlayerLocation = FVector::ZeroVector;
    if (APlayerController* PC = World->GetFirstPlayerController())
    {
        if (APawn* PlayerPawn = PC->GetPawn()) { PlayerPawn->DisableInput(PC); }
        if (CinematicCamera) { PC->SetViewTargetWithBlend(CinematicCamera, CameraBlendTime); }
    }

    // Pass the death location into a timer delegate to create the dramatic pause
    FTimerDelegate TimerDel;
    TimerDel.BindUFunction(this, FName("ExecutePortalSpawn"), BossDeathLocation);
    World->GetTimerManager().SetTimer(SequenceTimerHandle, TimerDel, SpawnDelay, false);
}

void ABossPortalDirector::ExecutePortalSpawn(FVector BossDeathLocation)
{
    UWorld* World = GetWorld();
    if (!World) return;

    FVector PlayerLocation = FVector::ZeroVector;
    if (APlayerController* PC = World->GetFirstPlayerController())
    {
        if (APawn* PlayerPawn = PC->GetPawn()) { PlayerLocation = PlayerPawn->GetActorLocation(); }
    }

    FVector FinalSpawnLocation = BossDeathLocation; // Fallback

    if (bUseManualAnchors && ManualPortalAnchors.Num() > 0)
    {
        // Evaluate Designer-Placed Anchors
        float MaxDistanceToPlayer = -1.0f;
        FVector BestAnchorLocation = GetTransform().TransformPosition(ManualPortalAnchors[0]);

        for (const FVector& AnchorOffset : ManualPortalAnchors)
        {
            FVector WorldAnchor = GetTransform().TransformPosition(AnchorOffset);

            // Calculate distance between this specific anchor and the active player
            float DistToPlayer = FVector::Dist(WorldAnchor, PlayerLocation);

            // If this anchor is further away than the previous best, overwrite it
            if (DistToPlayer > MaxDistanceToPlayer)
            {
                MaxDistanceToPlayer = DistToPlayer;
                BestAnchorLocation = WorldAnchor;
            }
        }

        FinalSpawnLocation = BestAnchorLocation;
    }
    else
    {
        // Procedural Random Shape Buffer (Existing Logic)
        float RandPct = FMath::RandRange(0.0f, 1.0f);
        float RandDistance = FMath::RandRange(MinSpawnRadius, MaxSpawnRadius);

        // Decide whther to center the math on the boss's loot, or the level designer's static room center
        FVector BaseCenter = bCenterOnBossDeath ? BossDeathLocation : GetTransform().TransformPosition(PortalCenterOffset);
        FinalSpawnLocation = BaseCenter + CalculateShapeOffset(RandPct, RandDistance);
    }

    if (bProjectToNavMesh)
    {
        if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World))
        {
            FNavLocation ProjectedLocation;
            if (NavSys->ProjectPointToNavigation(FinalSpawnLocation, ProjectedLocation, FVector(500.0f, 500.0f, 500.0f)))
            {
                FinalSpawnLocation = ProjectedLocation.Location;
            }
        }
    }

    FVector DirectionToPlayer = PlayerLocation - FinalSpawnLocation;
    DirectionToPlayer.Z = 0.0f;
    FRotator PortalRotation = DirectionToPlayer.IsNearlyZero() ? FRotator::ZeroRotator : DirectionToPlayer.Rotation();

    if (PortalClassToSpawn)
    {
        World->SpawnActor<AActor>(PortalClassToSpawn, FinalSpawnLocation, PortalRotation);
    }

    if (PortalSpawnVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, PortalSpawnVFX, FinalSpawnLocation, FRotator::ZeroRotator);
    }

    // Unlock the Arena Doors
    for (AActor* Door : LinkedArenaDoors)
    {
        if (Door)
        {
            Door->SetActorHiddenInGame(true);
            Door->SetActorEnableCollision(false);
        }
    }

    // Hold the camera on the portal, then return control
    World->GetTimerManager().SetTimer(SequenceTimerHandle, this, &ABossPortalDirector::RestorePlayerControl, PostSpawnCameraHoldTime, false);
}

// ==========================================
// CINEMATIC SEQUENCE STEP 3: RESTORE CONTROL
// ==========================================
void ABossPortalDirector::RestorePlayerControl()
{
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PC = World->GetFirstPlayerController())
        {
            if (APawn* PlayerPawn = PC->GetPawn())
            {
                PC->SetViewTargetWithBlend(PlayerPawn, CameraBlendTime);
                PlayerPawn->EnableInput(PC);
            }
        }
    }
}

#if WITH_EDITOR
void ABossPortalDirector::TestSpawnLocation()
{
    UWorld* World = GetWorld();
    if (!World) return;

    // Fake the boss dying exactly at then director's location
    FVector FakeBossLocation = GetActorLocation();
    FVector FinalSpawnLocation = FakeBossLocation;

    if (bUseManualAnchors && ManualPortalAnchors.Num() > 0)
    {
        FinalSpawnLocation = GetTransform().TransformPosition(ManualPortalAnchors[0]);
    }
    else
    {
        float RandPct = FMath::RandRange(0.0f, 1.0f);
        float RandDistance = FMath::RandRange(MinSpawnRadius, MaxSpawnRadius);
        FVector BaseCenter = bCenterOnBossDeath ? FakeBossLocation : GetTransform().TransformPosition(PortalCenterOffset);
        FinalSpawnLocation = BaseCenter + CalculateShapeOffset(RandPct, RandDistance);
    }

    if (bProjectToNavMesh)
    {
        if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World))
        {
            FNavLocation ProjectedLocation;
            if (NavSys->ProjectPointToNavigation(FinalSpawnLocation, ProjectedLocation, FVector(500.0f, 500.0f, 500.0f)))
            {
                FinalSpawnLocation = ProjectedLocation.Location;
            }
        }
    }

    // Draw gold sphere that lasts for 5 seconds to simulate the portal
    DrawDebugSphere(World, FinalSpawnLocation, 50.0f, 12, FColor(255, 215, 0), false, 5.0f, 0, 3.0f);
    UE_LOG(LogTemp, Warning, TEXT("Portal Test Spawned at: %s"), *FinalSpawnLocation.ToString());

}
#endif
