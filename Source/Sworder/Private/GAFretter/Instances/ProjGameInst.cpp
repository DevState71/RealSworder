// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/Instances/ProjGameInst.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "AudioDevice.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "GAFretter/Settings/SworderGameUserSettings.h"
#include "GameFramework/PlayerController.h"

void UProjGameInst::Init()
{
	Super::Init();

	if (USworderGameUserSettings* Settings = USworderGameUserSettings::GetSworderUserSettings())
	{
		// Safely fetch the core Audio Device directly from the Engine (bypassing GetWorld)
		if (GEngine)
		{
			if (FAudioDeviceHandle AudioDevice = GEngine->GetMainAudioDevice())
			{
				// Apply the SoundMix to the Audio Device
				if (MainSoundMix)
				{
					AudioDevice->PushSoundMixModifier(MainSoundMix);

					// Calculate perceptual volumes and apply them to the classes natively
					if (MasterSoundClass)
					{
						float PerpetualMaster = Settings->MasterVolume * Settings->MasterVolume;
						AudioDevice->SetSoundMixClassOverride(MainSoundMix, MasterSoundClass, PerpetualMaster, 1.0f, 0.0f, true);
					}
					if (MusicSoundClass)
					{
						float PerpetualMusic = Settings->MusicVolume * Settings->MusicVolume;
						AudioDevice->SetSoundMixClassOverride(MainSoundMix, MusicSoundClass, PerpetualMusic, 1.0f, 0.0f, true);
					}
					if (SFXSoundClass)
					{
						float PerpetualSFX = Settings->SFXVolume * Settings->SFXVolume;
						AudioDevice->SetSoundMixClassOverride(MainSoundMix, SFXSoundClass, PerpetualSFX, 1.0f, 0.0f, true);
					}
					if (DialogueSoundClass)
					{
						float PerpetualDialogue = Settings->DialogueVolume * Settings->DialogueVolume;
						AudioDevice->SetSoundMixClassOverride(MainSoundMix, DialogueSoundClass, PerpetualDialogue, 1.0f, 0.0f, true);
					}
				}
			}
		}
	}
}

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
