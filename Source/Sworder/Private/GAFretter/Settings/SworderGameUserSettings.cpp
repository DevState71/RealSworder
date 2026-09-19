// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/Settings/SworderGameUserSettings.h"
#include "Engine/Engine.h"

USworderGameUserSettings::USworderGameUserSettings()
{
	// Fallback Defaults
	MasterVolume = 1.0f;
	MusicVolume = 0.5f;
	SFXVolume = 0.5f;
	DialogueVolume = 0.5f;
	bGamepadVibration = true;
}

USworderGameUserSettings* USworderGameUserSettings::GetSworderUserSettings()
{
	if (GEngine) return Cast<USworderGameUserSettings>(GEngine->GetGameUserSettings());
	return nullptr;
}
