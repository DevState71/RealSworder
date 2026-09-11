// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/Instances/ProjGameInst.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

void UProjGameInst::LoadFirstLevel()
{
	LoadLevelSafe(FirstLevelIndex);
}

void UProjGameInst::LoadLevelSafe(int32 LevelIndex)
{
	// Safety check: Does this index exist in our array?
	if (GameLevels.IsValidIndex(LevelIndex))
	{
		// Cache the current level for later use (like the "Play Again" button)
		CurrentLevelIndex = LevelIndex;

		// Get the level name from the array
		FName LevelName = GameLevels[LevelIndex];

		// Transition the level without using UGameplayStatics
		if (UWorld* World = GetWorld())
		{
			if (APlayerController* PC = World->GetFirstPlayerController())
			{
				PC->ClientTravel(LevelName.ToString(), ETravelType::TRAVEL_Absolute);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error: The index %d is not a valid index in the GameLevels array!"), LevelIndex);
	}
}

void UProjGameInst::QuitTheGame()
{
	// Quit without using UGameplayStatics
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			PC->ConsoleCommand("quit");
		}
	}
}
