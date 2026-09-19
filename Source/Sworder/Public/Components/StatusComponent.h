// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "StatusComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActorDifferentiation, FGameplayTag, Tag, bool, bAdded);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Numbers")
	float BurnDamageTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Numbers")
	float BurnDamageTickInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Numbers")
	float ShieldAmount;

	FTimerHandle BurnTimerHandle;
	FTimerHandle SlowTimerHandle;
	FTimerHandle StunTimerHandle;
	FTimerHandle ShieldTimerHandle;

	FTimerHandle BurnDamageTickHandle;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Status")
	void HandleStatusTag(FGameplayTag tag, bool bAdded);

	UPROPERTY(BlueprintAssignable, Category = "Status")
	FActorDifferentiation AddStatusTag;
	
	void AddBurn();
	void RemoveBurn();
	void BurnDamageTickFunction();
	void AddSlow();
	void RemoveSlow();
	void AddStun();
	void RemoveStun();
	void AddShield();
	void RemoveShield();
};
