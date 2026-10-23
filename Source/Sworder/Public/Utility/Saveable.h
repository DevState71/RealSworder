// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Saveable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SWORDER_API ISaveable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SaveData(class USworderSaveGame* SaveGameRef);
	virtual void SaveData_Implementation(class USworderSaveGame* SaveGameRef) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LoadData(class USworderSaveGame* SaveGameRef);
	virtual void LoadData_Implementation(class USworderSaveGame* SaveGameRef) = 0;
};
