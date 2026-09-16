// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "StatusComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDER_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

	

public:	
	// Sets default values for this component's properties
	UStatusComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float BurnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float SlowTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float StunTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float ShieldTimer;

	FTimerHandle BurnTimerHandle;
	FTimerHandle SlowTimerHandle;
	FTimerHandle StunTimerHandle;
	FTimerHandle ShieldTimerHandle;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void HandleStatusTag(FGameplayTag tag, bool bAdded);

	
	void AddBurn();
	void RemoveBurn();
	void AddSlow();
	void RemoveSlow();
	void AddStun();
	void RemoveStun();
	void AddShield();
	void RemoveShield();
};
