// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "SworderGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class SWORDER_API USworderGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	USworderGameUserSettings();

	// Native static getter to bypass Kismet's UGameplayStatics
	static USworderGameUserSettings* GetSworderUserSettings();

	// --- AUDIO SETTINGS ---
	UPROPERTY(Config)
	float MasterVolume;

	UPROPERTY(Config)
	float MusicVolume;

	UPROPERTY(Config)
	float SFXVolume;

	UPROPERTY(Config)
	float DialogueVolume;

	// --- CONTROLS SETTINGS ---
	UPROPERTY(Config)
	bool bGamepadVibration;

	UPROPERTY(Config)
	FString PreferredInputDevice;
};
