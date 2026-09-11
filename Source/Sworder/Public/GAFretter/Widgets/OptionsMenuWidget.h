// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"
#include "GameFramework/GameUserSettings.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "OptionsMenuWidget.generated.h"

class UButtonWithText;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class SWORDER_API UOptionsMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	// The Switcher that acts like web-browser tabs
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* OptionsSwitcher;

	// Navigation Sidebar Buttons
	UPROPERTY(meta = (BindWidget))
	UButtonWithText* ControlsTabButton;

	UPROPERTY(meta = (BindWidget))
	UButtonWithText* AudioTabButton;

	UPROPERTY(meta = (BindWidget))
	UButtonWithText* GraphicsTabButton;

	UPROPERTY(meta = (BindWidget))
	UButtonWithText* BackButton;

	// Click Handlers
	UFUNCTION()
	void OnControlsTabClicked();

	UFUNCTION()
	void OnAudioTabClicked();

	UFUNCTION()
	void OnGraphicsTabClicked();

	UFUNCTION()
	void OnBackClicked();

	// ======== CONTROLS SETTINGS ========
	// --- CONTROLS WIDGETS ---
	UPROPERTY(meta = (BindWidget))
	UButton* KeybindsButton;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* GamepadVibCheckBox;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* InputDeviceComboBox;

	// --- CONTROLS EVENT HANDLERS ---
	UFUNCTION()
	void OnKeybindsClicked();

	UFUNCTION()
	void OnGamepadVibChanged(bool bIsChecked);

	UFUNCTION()
	void OnInputDeviceChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	// ======== AUDIO SETTINGS ========
	// --- UI SLIDERS ---
	UPROPERTY(meta = (BindWidget))
	USlider* MasterVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	USlider* MusicVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	USlider* SFXVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	USlider* DialogueVolumeSlider;

	// --- SOUND CLASS ASSET POINTERS ---
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundMix* MainSoundMix;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundClass* MasterSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundClass* MusicSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundClass* SFXSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundClass* DialogueSoundClass;

	// --- SLIDER EVENT HANDLERS ---
	UFUNCTION()
	void OnMasterVolumeChanged(float Value);

	UFUNCTION()
	void OnMusicVolumeChanged(float Value);

	UFUNCTION()
	void OnSFXVolumeChanged(float Value);

	UFUNCTION()
	void OnDialogueVolumeChanged(float Value);

	// ====== GRAPHICS SETTINGS ========
	// --- GRAPHICS WIDGETS ---
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* DisplayModeComboBox;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* VSyncCheckBox;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ResolutionComboBox;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* FramerateComboBox;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* AntiAliasingComboBox;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* QualityPresetComboBox;

	// --- GRAPHICS EVENT HANDLERS ---
	UFUNCTION()
	void OnDisplayModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnVSyncChanged(bool bIsChecked);

	UFUNCTION()
	void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnFramerateChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnAntiAliasingChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnQualityPresetChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
};
