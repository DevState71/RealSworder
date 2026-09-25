// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ComboDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FElementCombo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	FGameplayTag Result;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	TArray<FGameplayTag> ComboSequence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	bool bEndsCombo = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	bool SelfOrEnemy = false;
};

UCLASS(BlueprintType, Blueprintable)
class SWORDER_API UComboDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combos")
	TArray<FElementCombo> StoredCombos;

	UFUNCTION(CallInEditor, Category = "Combo Generation")
	void GenerateCombos();

	void AddCombo(
		const TArray<FGameplayTag>& Sequence,
		FGameplayTag Result,
		bool bSelfTarget,
		bool bEndsCombo
	);



};
