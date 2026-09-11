// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/Widgets/OptionsMenuWidget.h"
#include "GAFretter/Widgets/ButtonWithText.h"
#include "AudioDevice.h"
#include "Characters/BasePlayer.h"
#include "Components/WidgetSwitcher.h"

class ABasePlayer;

void UOptionsMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (ControlsTabButton) ControlsTabButton->SetButtonText(FText::FromString("Controls"));
	if (AudioTabButton) AudioTabButton->SetButtonText(FText::FromString("Audio"));
	if (GraphicsTabButton) GraphicsTabButton->SetButtonText(FText::FromString("Graphics"));
	if (BackButton) BackButton->SetButtonText(FText::FromString("Back"));
}

void UOptionsMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind delegates
	if (ControlsTabButton) ControlsTabButton->OnButtonClicked.AddDynamic(this, &UOptionsMenuWidget::OnControlsTabClicked);
	if (AudioTabButton) AudioTabButton->OnButtonClicked.AddDynamic(this, &UOptionsMenuWidget::OnAudioTabClicked);
	if (GraphicsTabButton) GraphicsTabButton->OnButtonClicked.AddDynamic(this, &UOptionsMenuWidget::OnGraphicsTabClicked);
	if (BackButton) BackButton->OnButtonClicked.AddDynamic(this, &UOptionsMenuWidget::OnBackClicked);
	
	// --- CONTROLS BINDINGS ---
	if (KeybindsButton)
	{
		// Note: UButton uses OnClicked instead of OnButtonClicked natively depending on the class, 
		// but since this uses OnButtonClicked for the custom Tab Buttons, use standard OnClicked for native UButtons.
		KeybindsButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::OnKeybindsClicked);
	}

	if (GamepadVibCheckBox)
	{
		GamepadVibCheckBox->OnCheckStateChanged.AddDynamic(this, &UOptionsMenuWidget::OnGamepadVibChanged);
	}

	if (InputDeviceComboBox)
	{
		InputDeviceComboBox->AddOption("Auto-Detect");
		InputDeviceComboBox->AddOption("Keyboard / Mouse");
		InputDeviceComboBox->AddOption("Controller");
		InputDeviceComboBox->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::OnInputDeviceChanged);
	}

	// --- AUDIO BINDINGS ---
	if (MasterVolumeSlider)
	{
		MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionsMenuWidget::OnMasterVolumeChanged);
		if (MasterSoundClass) MasterVolumeSlider->SetValue(MasterSoundClass->Properties.Volume);
	}
	if (MusicVolumeSlider)
	{
		MusicVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionsMenuWidget::OnMusicVolumeChanged);
		if (MusicSoundClass) MusicVolumeSlider->SetValue(MusicSoundClass->Properties.Volume);
	}
	if (SFXVolumeSlider)
	{
		SFXVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionsMenuWidget::OnSFXVolumeChanged);
		if (SFXSoundClass) SFXVolumeSlider->SetValue(SFXSoundClass->Properties.Volume);
	}
	if (DialogueVolumeSlider)
	{
		DialogueVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionsMenuWidget::OnDialogueVolumeChanged);
		if (DialogueSoundClass) DialogueVolumeSlider->SetValue(DialogueSoundClass->Properties.Volume);
	}

	// --- GRAPHICS BINDINGS ---
	if (DisplayModeComboBox)
	{
		DisplayModeComboBox->AddOption("Fullscreen");
		DisplayModeComboBox->AddOption("Windowed Fullscreen");
		DisplayModeComboBox->AddOption("Windowed");
		DisplayModeComboBox->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::OnDisplayModeChanged);
	}

	if (VSyncCheckBox) VSyncCheckBox->OnCheckStateChanged.AddDynamic(this, &UOptionsMenuWidget::OnVSyncChanged);

	if (ResolutionComboBox)
	{
		ResolutionComboBox->AddOption("1280x720");
		ResolutionComboBox->AddOption("1920x1080");
		ResolutionComboBox->AddOption("2560x1440");
		ResolutionComboBox->AddOption("3840x2160");
		ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::OnResolutionChanged);
	}

	if (FramerateComboBox)
	{
		FramerateComboBox->AddOption("30");
		FramerateComboBox->AddOption("60");
		FramerateComboBox->AddOption("120");
		FramerateComboBox->AddOption("144");
		FramerateComboBox->AddOption("Uncapped");
		FramerateComboBox->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::OnFramerateChanged);
	}

	if (AntiAliasingComboBox)
	{
		AntiAliasingComboBox->AddOption("Low");
		AntiAliasingComboBox->AddOption("Medium");
		AntiAliasingComboBox->AddOption("High");
		AntiAliasingComboBox->AddOption("Epic");
		AntiAliasingComboBox->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::OnAntiAliasingChanged);
	}

	if (QualityPresetComboBox)
	{
		QualityPresetComboBox->AddOption("Low");
		QualityPresetComboBox->AddOption("Medium");
		QualityPresetComboBox->AddOption("High");
		QualityPresetComboBox->AddOption("Epic");
		QualityPresetComboBox->OnSelectionChanged.AddDynamic(this, &UOptionsMenuWidget::OnQualityPresetChanged);
	}
}

void UOptionsMenuWidget::OnControlsTabClicked() { if (OptionsSwitcher) OptionsSwitcher->SetActiveWidgetIndex(0); }

void UOptionsMenuWidget::OnAudioTabClicked() { if (OptionsSwitcher) OptionsSwitcher->SetActiveWidgetIndex(1); }

void UOptionsMenuWidget::OnGraphicsTabClicked() { if (OptionsSwitcher) OptionsSwitcher->SetActiveWidgetIndex(2); }

	// Remove this menu to reveal the Main Menu or Pause Menu sitting behind it
void UOptionsMenuWidget::OnBackClicked() { RemoveFromParent(); }

// ================ CONTROLS EVENT HANDLERS =================

void UOptionsMenuWidget::OnKeybindsClicked()
{
	// TODO: Spawn and AddToViewport your separate Keybindings Sub-Menu Widget
}

void UOptionsMenuWidget::OnGamepadVibChanged(bool bIsChecked)
{
	// TODO: Toggle Force Feedback booleans in PlayerController
}

void UOptionsMenuWidget::OnInputDeviceChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	// TODO: Override UI navigation rules and input icons based on SelectedItem
}

// ================ AUDIO SLIDER EVENT HANDLERS =================
void UOptionsMenuWidget::OnMasterVolumeChanged(float Value)
{
	float PerceptualVolume = Value * Value;

	if (MainSoundMix && MasterSoundClass)
	{
		if (UWorld* World = GetWorld())
		{
			// Natively fetch the active audio device handle
			if (FAudioDeviceHandle AudioDevice = World->GetAudioDevice())
			{
				AudioDevice->PushSoundMixModifier(MainSoundMix);
				AudioDevice->SetSoundMixClassOverride(MainSoundMix, MasterSoundClass, PerceptualVolume, 1.0f, 0.0f, true);
			}
		}
	}
}

void UOptionsMenuWidget::OnMusicVolumeChanged(float Value)
{
	float PerceptualVolume = Value * Value;

	if (MainSoundMix && MusicSoundClass)
	{
		if (UWorld* World = GetWorld())
		{
			// Natively fetch the active audio device handle
			if (FAudioDeviceHandle AudioDevice = World->GetAudioDevice())
			{
				AudioDevice->PushSoundMixModifier(MainSoundMix);
				AudioDevice->SetSoundMixClassOverride(MainSoundMix, MusicSoundClass, PerceptualVolume, 1.0f, 0.0f, true);
			}
		}
	}
}

void UOptionsMenuWidget::OnSFXVolumeChanged(float Value)
{
	float PerceptualVolume = Value * Value;

	if (MainSoundMix && SFXSoundClass)
	{
		if (UWorld* World = GetWorld())
		{
			// Natively fetch the active audio device handle
			if (FAudioDeviceHandle AudioDevice = World->GetAudioDevice())
			{
				AudioDevice->PushSoundMixModifier(MainSoundMix);
				AudioDevice->SetSoundMixClassOverride(MainSoundMix, SFXSoundClass, PerceptualVolume, 1.0f, 0.0f, true);
			}
		}
	}
}

void UOptionsMenuWidget::OnDialogueVolumeChanged(float Value)
{
	float PerceptualVolume = Value * Value;

	if (MainSoundMix && DialogueSoundClass)
	{
		if (UWorld* World = GetWorld())
		{
			// Natively fetch the active audio device handle
			if (FAudioDeviceHandle AudioDevice = World->GetAudioDevice())
			{
				AudioDevice->PushSoundMixModifier(MainSoundMix);
				AudioDevice->SetSoundMixClassOverride(MainSoundMix, DialogueSoundClass, PerceptualVolume, 1.0f, 0.0f, true);
			}
		}
	}
}

// ================ GRAPHICS EVENT HANDLERS =================
void UOptionsMenuWidget::OnDisplayModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (UGameUserSettings* UserSettings = GEngine->GetGameUserSettings())
	{
		if (SelectedItem == "Fullscreen") UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
		else if (SelectedItem == "Windowed Fullscreen") UserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
		else if (SelectedItem == "Windowed") UserSettings->SetFullscreenMode(EWindowMode::Windowed);

		UserSettings->ApplySettings(false);
	}
}

void UOptionsMenuWidget::OnVSyncChanged(bool bIsChecked)
{
	if (UGameUserSettings* UserSettings = GEngine->GetGameUserSettings())
	{
		UserSettings->SetVSyncEnabled(bIsChecked);
		UserSettings->ApplySettings(false);
	}
}

void UOptionsMenuWidget::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (UGameUserSettings* UserSettings = GEngine->GetGameUserSettings())
	{
		FString LeftSide, RightSide;
		// Split the string into two separate numbers
		if (SelectedItem.Split(TEXT("x"), &LeftSide, &RightSide))
		{
			int32 ResX = FCString::Atoi(*LeftSide);
			int32 ResY = FCString::Atoi(*RightSide);

			UserSettings->SetScreenResolution(FIntPoint(ResX, ResY));
			UserSettings->ApplySettings(false);
		}
	}
}

void UOptionsMenuWidget::OnFramerateChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (UGameUserSettings* UserSettings = GEngine->GetGameUserSettings())
	{
		if (SelectedItem == "Uncapped")
		{
			// 0.0f tells Unreal to uncap the framerate
			UserSettings->SetFrameRateLimit(0.0f);
		}
		else
		{
			// Convert the string directly to a float
			UserSettings->SetFrameRateLimit(FCString::Atof(*SelectedItem));
		}

		UserSettings->ApplySettings(false);
	}
}

void UOptionsMenuWidget::OnAntiAliasingChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (UGameUserSettings* UserSettings = GEngine->GetGameUserSettings())
	{
		int32 QualityLevel = 3; // Default to Epic
		if (SelectedItem == "Low") QualityLevel = 0;
		else if (SelectedItem == "Medium") QualityLevel = 1;
		else if (SelectedItem == "High") QualityLevel = 2;

		UserSettings->SetAntiAliasingQuality(QualityLevel);
		UserSettings->ApplySettings(false);
	}
}

void UOptionsMenuWidget::OnQualityPresetChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (UGameUserSettings* UserSettings = GEngine->GetGameUserSettings())
	{
		int32 QualityLevel = 3; // Default to Epic
		if (SelectedItem == "Low") QualityLevel = 0;
		else if (SelectedItem == "Medium") QualityLevel = 1;
		else if (SelectedItem == "High") QualityLevel = 2;

		// This master function instantly cascades the 0-3 level down to Shadows, Textures, Effects, etc.
		UserSettings->SetOverallScalabilityLevel(QualityLevel);
		UserSettings->ApplySettings(false);
	}
}
