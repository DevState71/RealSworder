// Fill out your copyright notice in the Description page of Project Settings.


#include "FormationTrapPlate.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AFormationTrapPlate::AFormationTrapPlate()
{
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

void AFormationTrapPlate::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && EnemyClassToSpawn)
    {
        TriggerZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        FVector Center = GetActorLocation();

        // 0: Circle, 1: Square, 2: Triangle
        int32 Type = FMath::RandRange(0, 2);

        for (int32 i = 0; i < 8; ++i)
        {
            float Angle = (TWO_PI / 8.0f) * i;
            float XOffset = 0.0f, YOffset = 0.0f;

            if (Type == 0) {
                XOffset = FMath::Cos(Angle) * FormationRadius;
                YOffset = FMath::Sin(Angle) * FormationRadius;
            }
            else if (Type == 1) {
                XOffset = (i < 2 || i > 5) ? FormationRadius : -FormationRadius;
                YOffset = (i % 4 < 2) ? FormationRadius : -FormationRadius;
            }
            else {
                XOffset = (i % 3 == 0) ? 0 : (i % 3 == 1 ? FormationRadius : -FormationRadius);
                YOffset = (i % 3 == 0) ? FormationRadius : -FormationRadius;
            }

            FVector SpawnLocation = Center + FVector(XOffset, YOffset, 0.0f);
            GetWorld()->SpawnActor<AActor>(EnemyClassToSpawn, SpawnLocation, FRotator::ZeroRotator);
        }
    }
}

