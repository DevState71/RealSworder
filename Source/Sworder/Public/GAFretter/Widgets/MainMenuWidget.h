// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundBase.h"
#include "MainMenuWidget.generated.h"

class UButtonWithText;
class UProjGameInst;
class USoundBase;

/**
 * 
 */
UCLASS()
class SWORDER_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativePreConstruct() override;
	// This runs when the widget is created, similar to BeginPlay
	virtual void NativeConstruct() override;

	// These variables MUST exactly match the names of your buttons in the UMG Blueprint
	UPROPERTY(meta = (BindWidget))
	UButtonWithText* PlayButton;

	UPROPERTY(meta = (BindWidget))
	UButtonWithText* QuitButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundBase* MenuMusic;

	// The Options button on Main Menu
	UPROPERTY(meta = (BindWidget))
	UButtonWithText* OptionsButton;

	// The class reference to spawn the Options Menu
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> OptionsMenuClass;

	UFUNCTION()
	void OnOptionsClicked();

private:
	// The functions that run when the buttons are clicked
	UFUNCTION()
	void OnPlayClicked();

	UFUNCTION()
	void OnQuitClicked();
};
