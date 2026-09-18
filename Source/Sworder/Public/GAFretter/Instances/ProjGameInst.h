// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/Texture2D.h"
#include "ProjGameInst.generated.h"

class USoundMix;
class USoundClass;

USTRUCT(BlueprintType)
struct FDeviceIconSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ConfirmButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* CancelButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* PrimaryAttack;

};

/**
 * 
 */
UCLASS()
class SWORDER_API UProjGameInst : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	// Core transition functions
	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void LoadFirstLevel();

	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void QuitTheGame();

	// The safe loading function that checks array bounds
	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void LoadLevelSafe(int32 LevelIndex);

	// Called by the Options Menu to swap the active texture Set to the appropriate input device in question.
	UFUNCTION(BlueprintCallable, Category = "Input Settings")
	void UpdateInputDevicePreference(FString DeviceName);

	UFUNCTION(BlueprintCallable, Category = "Input Settings")
	FDeviceIconSet GetActiveIconSet() const { return ActiveIconSet; }

protected:
	// -------- LEVEL DATA --------
	// Holds the names of our maps (e.g., "MainMenu", "TestingMap")
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level Data")
	TArray<FName> GameLevels;

	// The index for the first gameplay level
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level Data")
	int32 FirstLevelIndex = 1;

	// Caches the level we are currently on
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level Data")
	int32 CurrentLevelIndex = 0;

	// -------- NATIVE AUDIO ASSET POINTERS --------
	UPROPERTY(EditDefaultsOnly, Category = "Audio|Mixes")
	USoundMix* MainSoundMix;

	UPROPERTY(EditDefaultsOnly, Category = "Audio|Classes")
	USoundClass* MasterSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "Audio|Classes")
	USoundClass* MusicSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "Audio|Classes")
	USoundClass* SFXSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "Audio|Classes")
	USoundClass* DialogueSoundClass;

	// -------- INPUT DEVICE ICON SETS --------

	// Maps the device string to the appropriate icon set, based on the current input device in question
	UPROPERTY(EditDefaultsOnly, Category = "UI|Input Icons")
	TMap<FString, FDeviceIconSet> DeviceIconDictionary;

	// Current active set of icons loaded in memory
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|Input Icons")
	FDeviceIconSet ActiveIconSet;
};
