// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TagManager.h"
#include "Components/HealthComponent.h"
#include "ElementalAOE.generated.h"

class USphereComponent;

UCLASS()
class SWORDER_API AElementalAOE : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElementalAOE();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	float Radius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	FGameplayTag StatusToApply;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	TObjectPtr<AActor> EffectSource;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	TObjectPtr<AActor> ComboSource;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "AOE")
	void InitializeAOE(AActor* InEffectSource, AActor* InComboSource, float InDamage, float InRadius, FGameplayTag InStatusToApply);

	UFUNCTION(BlueprintCallable, Category = "AOE")
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ProcessOverlap(AActor* OtherActor, FGameplayTag InStatusToApply);

};
