// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class SWORDER_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBoxComponent* DamageCollision;

	
	APawn* ParentPawn;

	// Weapon Variables
	float Damage;
	float SliceSpeed;
	float SheathSpeed;
	float Length;
	int SlashCount;
	float RecoverySpeed;

	UFUNCTION(BlueprintCallable)
	void SetWeaponVariables(float newDamage, float newSliceSpeed, float newSheathSpeed, float newLength, int newSlashCount, float newRecoverySpeed);

	UFUNCTION()
	void WeaponCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void SetParentPawn(APawn* Parent);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
