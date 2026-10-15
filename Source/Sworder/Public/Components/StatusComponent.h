// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "StatusComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActorDifferentiation, FGameplayTag, Tag, bool, bAdded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FStatusEffectAreaRequested, FGameplayTag, Tag, bool, bAdded, AActor*, ComboCauser, AActor*, EffectSource, FVector, EffectLocation);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDER_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

	

public:	
	// Sets default values for this component's properties
	UStatusComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float BlizzardTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float BurnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float BurstTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float CharTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float ChargeTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float ConductorTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float CrystalizeTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float EarthquakeTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float EmberTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float EnergizeTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float EntombTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float ExplodeTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float ExtinguishTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float FractureTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float FreezeTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float HyperchargeTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float LavaTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float MagmaTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float MeltTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float OverchargeTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float ShieldTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float ShockwaveTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float SingeTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float SlowTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float SmolderTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float StaggerTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float StormTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float StunTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float SuperchargeTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float SuperheatTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float ThunderboltTimer;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Numbers")
	float BurnDamageTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Numbers")
	float BurnDamageTickInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Numbers")
	float ShieldAmount;

	FTimerHandle BlizzardTimerHandle;
	FTimerHandle BurnTimerHandle;
	FTimerHandle BurstTimerHandle;
	FTimerHandle CharTimerHandle;
	FTimerHandle ChargeTimerHandle;
	FTimerHandle ConductorTimerHandle;
	FTimerHandle CrystalizeTimerHandle;
	FTimerHandle EarthquakeTimerHandle;
	FTimerHandle EmberTimerHandle;
	FTimerHandle EnergizeTimerHandle;
	FTimerHandle EntombTimerHandle;
	FTimerHandle ExplodeTimerHandle;
	FTimerHandle ExtinguishTimerHandle;
	FTimerHandle FractureTimerHandle;
	FTimerHandle FreezeTimerHandle;
	FTimerHandle HyperchargeTimerHandle;
	FTimerHandle LavaTimerHandle;
	FTimerHandle MagmaTimerHandle;
	FTimerHandle MeltTimerHandle;
	FTimerHandle OverchargeTimerHandle;
	FTimerHandle ShieldTimerHandle;
	FTimerHandle ShockwaveTimerHandle;
	FTimerHandle SingeTimerHandle;
	FTimerHandle SlowTimerHandle;
	FTimerHandle SmolderTimerHandle;
	FTimerHandle StaggerTimerHandle;
	FTimerHandle StormTimerHandle;
	FTimerHandle StunTimerHandle;
	FTimerHandle SuperchargeTimerHandle;
	FTimerHandle SuperheatTimerHandle;
	FTimerHandle ThunderboltTimerHandle;

	FTimerHandle BurnDamageTickHandle;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Status")
	void HandleStatusTag(FGameplayTag tag, bool bAdded, AActor* ComboCauser);

	UPROPERTY(BlueprintAssignable, Category = "Status")
	FActorDifferentiation AddStatusTag;
	
	UPROPERTY(BlueprintAssignable, Category = "Status")
	FStatusEffectAreaRequested OnStatusEffectAreaRequested;

	void AddBlizzard();
	void RemoveBlizzard();

	void AddBurn();
	void RemoveBurn();
	void BurnDamageTickFunction();

	void AddBurst();
	void RemoveBurst();

	void AddChar();
	void RemoveChar();

	void AddCharge();
	void RemoveCharge();

	void AddConductor();
	void RemoveConductor();

	void AddCrystalize();
	void RemoveCrystalize();

	void AddEarthquake();
	void RemoveEarthquake();

	void AddEmber(AActor* ComboCauser);
	void RemoveEmber();

	void AddEnergize();
	void RemoveEnergize();

	void AddEntomb();
	void RemoveEntomb();

	void AddExplode();
	void RemoveExplode();

	void AddExtinguish();
	void RemoveExtinguish();

	void AddFracture();
	void RemoveFracture();

	void AddFreeze();
	void RemoveFreeze();

	void AddHypercharge();
	void RemoveHypercharge();

	void AddLava();
	void RemoveLava();

	void AddMagma();
	void RemoveMagma();

	void AddMelt();
	void RemoveMelt();

	void AddOvercharge();
	void RemoveOvercharge();

	void AddShield();
	void RemoveShield();

	void AddShockwave();
	void RemoveShockwave();

	void AddSinge();
	void RemoveSinge();

	void AddSlow();
	void RemoveSlow();

	void AddSmolder();
	void RemoveSmolder();

	void AddStagger();
	void RemoveStagger();

	void AddStorm();
	void RemoveStorm();

	void AddStun();
	void RemoveStun();

	void AddSupercharge();
	void RemoveSupercharge();

	void AddSuperheat();
	void RemoveSuperheat();

	void AddThunderbolt();
	void RemoveThunderbolt();
};
