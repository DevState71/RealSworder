// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Components/ComboDataAsset.h"
#include "TagManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStatusTagging, FGameplayTag, Tag, bool, bAdded);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDER_API UTagManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTagManager();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer GameplayTagContainer;

	const FGameplayTagContainer& GetGameplayTagContainer() const { return GameplayTagContainer; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 StatusChance = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	UComboDataAsset* ComboDataAsset;

	UPROPERTY()
	TArray<FGameplayTag> ComboTags;

	FGameplayTag CurrentComboTag;

	FTimerHandle ComboTimerHandle;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StatusRoll(FGameplayTag tag);


	FElementCombo* ComboBuild(FGameplayTag tag);
	FElementCombo* TriggerCombo();
	bool CheckNeutral();
	void EndCombo();

		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	float ComboTime = 5.0f;

	UPROPERTY(BlueprintAssignable)
	FStatusTagging AddStatusTag;


	UFUNCTION(BlueprintCallable, Category = "Tags")
	void AddGameplayTag(FGameplayTag tag);
};
