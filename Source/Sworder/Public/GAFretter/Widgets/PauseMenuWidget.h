// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UButtonWithText;

/**
 * 
 */
UCLASS()
class SWORDER_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	// The BindWidget meta tag enforces these exact names in the Blueprint hierarchy
	UPROPERTY(meta = (BindWidget))
	UButtonWithText* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	UButtonWithText* OptionsButton;

	UPROPERTY(meta = (BindWidget))
	UButtonWithText* QuitButton;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> OptionsMenuClass;

	// Click Handlers
	UFUNCTION()
	void OnResumeClicked();

	UFUNCTION()
	void OnOptionsClicked();

	UFUNCTION()
	void OnQuitClicked();
};
