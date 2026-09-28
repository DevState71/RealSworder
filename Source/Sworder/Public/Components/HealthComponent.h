// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/ComboDataAsset.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboDiscovered, FElementCombo, DiscoveredCombo);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	// Exposing delegate, so the UI can listen to it
	UPROPERTY(BlueprintAssignable, Category = "Combo Discovery")
	FOnComboDiscovered OnComboDiscovered;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	float ProcessDamageType(float DamageAmount, AActor* DamageCauser);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void TakeFlatDamage(float DamageAmount, AActor* DamageCauser);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	float DamageShield(float DamageAmount, AActor* DamageCauser);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float ShieldHP;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
