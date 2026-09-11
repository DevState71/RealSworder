// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjGameInst.generated.h"

/**
 * 
 */
UCLASS()
class SWORDER_API UProjGameInst : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// Core transition functions
	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void LoadFirstLevel();

	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void QuitTheGame();

	// The safe loading function that checks array bounds
	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void LoadLevelSafe(int32 LevelIndex);

protected:
	// Holds the names of our maps (e.g., "MainMenu", "TestingMap")
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level Data")
	TArray<FName> GameLevels;

	// The index for the first gameplay level
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level Data")
	int32 FirstLevelIndex = 1;

	// Caches the level we are currently on
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level Data")
	int32 CurrentLevelIndex = 0;

};
