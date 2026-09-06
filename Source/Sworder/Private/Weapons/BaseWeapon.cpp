// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABaseWeapon::ABaseWeapon() : Damage(5)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMesh"));
	SetRootComponent(SkeletalMesh);
	DamageCollision = CreateDefaultSubobject<UBoxComponent>(FName("DamageCollision"));
	DamageCollision->SetupAttachment(SkeletalMesh);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::WeaponCollision);
}

void ABaseWeapon::SetWeaponVariables(float newDamage, float newSliceSpeed, float newSheathSpeed, float newLength, int newSlashCount, float newRecoverySpeed)
{
	Damage = newDamage;
	SliceSpeed = newSliceSpeed;
	SheathSpeed = newSheathSpeed;
	Length = newLength;
	SlashCount = newSlashCount;
	RecoverySpeed = newRecoverySpeed;
}

void ABaseWeapon::WeaponCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && ParentPawn) {
		UGameplayStatics::ApplyDamage(OtherActor, Damage, ParentPawn->GetController(), this, NULL);
	}

}

void ABaseWeapon::SetParentPawn(APawn* Parent)
{
	ParentPawn = Parent;
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

