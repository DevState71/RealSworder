// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/Widgets/OptionsMenuWidget.h"
#include "GAFretter/Widgets/ButtonWithText.h"
#include "GAFretter/Settings/SworderGameUserSettings.h"
#include "GAFretter/Instances/ProjGameInst.h"
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
		InputDeviceComboBox->AddOption("Xbox Series X");
		InputDeviceComboBox->AddOption("Xbox One");
		InputDeviceComboBox->AddOption("Xbox 360");
		InputDeviceComboBox->AddOption("PlayStation 4");
		InputDeviceComboBox->AddOption("PlayStation 5");
		InputDeviceComboBox->AddOption("Nintendo Switch Pro");
		InputDeviceComboBox->AddOption("Nintendo Switch 2 Pro");
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

	if (USworderGameUserSettings* Settings = USworderGameUserSettings::GetSworderUserSettings())
	{
		if (GamepadVibCheckBox) GamepadVibCheckBox->SetIsChecked(Settings->bGamepadVibration);
		if (InputDeviceComboBox && !Settings->PreferredInputDevice.IsEmpty()) InputDeviceComboBox->SetSelectedOption(Settings->PreferredInputDevice);
		if (MasterVolumeSlider) MasterVolumeSlider->SetValue(Settings->MasterVolume);
		if (MusicVolumeSlider) MusicVolumeSlider->SetValue(Settings->MusicVolume);
		if (SFXVolumeSlider) SFXVolumeSlider->SetValue(Settings->SFXVolume);
		if (DialogueVolumeSlider) DialogueVolumeSlider->SetValue(Settings->DialogueVolume);
		if (VSyncCheckBox) VSyncCheckBox->SetIsChecked(Settings->IsVSyncEnabled());

		if (ResolutionComboBox)
		{
			FIntPoint CurrentRes = Settings->GetScreenResolution();
			FString ResString = FString::Printf(TEXT("%dx%d"), CurrentRes.X, CurrentRes.Y);
			ResolutionComboBox->SetSelectedOption(ResString);
		}

		if (DisplayModeComboBox)
		{
			EWindowMode::Type CurrentMode = Settings->GetFullscreenMode();
			if (CurrentMode == EWindowMode::Fullscreen) DisplayModeComboBox->SetSelectedOption("Fullcreen");
			else if (CurrentMode == EWindowMode::WindowedFullscreen) DisplayModeComboBox->SetSelectedOption("Windowed Fullscreen");
			else DisplayModeComboBox->SetSelectedOption("Windowed");
		}

		if (QualityPresetComboBox)
		{
			int32 CurrentQuality = Settings->GetOverallScalabilityLevel();
			if (CurrentQuality == 0) QualityPresetComboBox->SetSelectedOption("Low");
			else if (CurrentQuality == 1) QualityPresetComboBox->SetSelectedOption("Medium");
			else if (CurrentQuality == 2) QualityPresetComboBox->SetSelectedOption("High");
			else QualityPresetComboBox->SetSelectedOption("Epic");
		}
	}
}

void UOptionsMenuWidget::OnControlsTabClicked() { if (OptionsSwitcher) OptionsSwitcher->SetActiveWidgetIndex(0); }

void UOptionsMenuWidget::OnAudioTabClicked() { if (OptionsSwitcher) OptionsSwitcher->SetActiveWidgetIndex(1); }

void UOptionsMenuWidget::OnGraphicsTabClicked() { if (OptionsSwitcher) OptionsSwitcher->SetActiveWidgetIndex(2); }

	// Remove this menu to reveal the Main Menu or Pause Menu sitting behind it
void UOptionsMenuWidget::OnBackClicked()
{ 
	if (USworderGameUserSettings* Settings = USworderGameUserSettings::GetSworderUserSettings())
	{
		if (MasterVolumeSlider) Settings->MasterVolume = MasterVolumeSlider->GetValue();
		if (MusicVolumeSlider) Settings->MusicVolume = MusicVolumeSlider->GetValue();
		if (SFXVolumeSlider) Settings->SFXVolume = SFXVolumeSlider->GetValue();
		if (DialogueVolumeSlider) Settings->DialogueVolume = DialogueVolumeSlider->GetValue();
		if (GamepadVibCheckBox) Settings->bGamepadVibration = GamepadVibCheckBox->IsChecked();
		Settings->SaveSettings();
	}
	RemoveFromParent();
}

// ================ CONTROLS EVENT HANDLERS =================

void UOptionsMenuWidget::OnKeybindsClicked()
{
	// This ensures the designer actually slotted a widget class in the editor
	if (KeybindsMenuClass)
	{
		// This natively constructs the widget
		if (UUserWidget* KeybindsMenu = CreateWidget<UUserWidget>(GetWorld(), KeybindsMenuClass))
		{
			// Adds it to the screen w/ Z-Order of 10 so it overlays on top of the Options Menu
			KeybindsMenu->AddToViewport(10);
		}
	}
}

void UOptionsMenuWidget::OnGamepadVibChanged(bool bIsChecked)
{
	// Fetch active player controller natively
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			PC->bForceFeedbackEnabled = bIsChecked;
		}
	}
}

void UOptionsMenuWidget::OnInputDeviceChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (USworderGameUserSettings* Settings = USworderGameUserSettings::GetSworderUserSettings())
	{
		// Saves the string value of the selected input device to the settings file for later retrieval
		Settings->PreferredInputDevice = SelectedItem;
		Settings->SaveSettings();

		// Safely casts our custom Game Instance
		if (UProjGameInst* GameInst = Cast<UProjGameInst>(GetGameInstance()))
		{
			// Updates the global icon dictionary
			GameInst->UpdateInputDevicePreference(SelectedItem);
		}
	}
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
