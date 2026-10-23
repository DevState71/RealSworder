// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/SworderSaveGame.h"
#include "Utility/Saveable.h"


USworderSaveGame::USworderSaveGame()
{
}

void USworderSaveGame::SaveAllData(TArray<AActor*> Actors)
{
	for (int i = 0; i < Actors.Num(); i++)
	{
		// Call Saveable interface function
		ISaveable* Interface = Cast<ISaveable>(Actors[i]);
		if (Interface) {
			Interface->SaveData(this);
		}
	}
}

void USworderSaveGame::LoadObjectData(AActor* Actor)
{
	ISaveable* Interface = Cast<ISaveable>(Actor);
	if (Interface) {
		Interface->LoadData(this);
	}
}
