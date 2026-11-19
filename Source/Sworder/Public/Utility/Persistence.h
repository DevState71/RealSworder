// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Persistence.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class UPersistence : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SWORDER_API IPersistence
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable)
	virtual void LoadActorsData(AActor* Actor) = 0;

	UFUNCTION(BlueprintCallable)
	virtual void RequestSave(bool bAsync) = 0;
};
