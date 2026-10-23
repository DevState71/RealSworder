// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SworderSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SWORDER_API USworderSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	USworderSaveGame();

	void SaveAllData(TArray<AActor*> Actors);
	void LoadObjectData(AActor* Actor);
};
