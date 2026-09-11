// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/Widgets/PauseMenuWidget.h"
#include "GAFretter/Widgets/ButtonWithText.h"
#include "GAFretter/Instances/ProjGameInst.h"

void UPauseMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (ResumeButton) ResumeButton->SetButtonText(FText::FromString("Resume"));
	if (OptionsButton) OptionsButton->SetButtonText(FText::FromString("Options"));
	if (QuitButton) QuitButton->SetButtonText(FText::FromString("Quit to Main Menu"));
}

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind custom button delegates here. 
	if (ResumeButton) ResumeButton->OnButtonClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);
	if (OptionsButton) OptionsButton->OnButtonClicked.AddDynamic(this, &UPauseMenuWidget::OnOptionsClicked);
	if (QuitButton) QuitButton->OnButtonClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitClicked);
}

void UPauseMenuWidget::OnResumeClicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		// Native Unpause
		PC->SetPause(false);

		// Hide the mouse and return input to gameplay natively
		FInputModeGameOnly InputModeData;
		PC->SetInputMode(InputModeData);
		PC->SetShowMouseCursor(false);
		PC->SetIgnoreLookInput(false);
		PC->SetIgnoreMoveInput(false);
	}

	// Destroy this widget
	RemoveFromParent();
}

void UPauseMenuWidget::OnOptionsClicked()
{
	if (OptionsMenuClass)
	{
		// Construct the options menu
		UUserWidget* OptionsMenu = CreateWidget<UUserWidget>(GetOwningPlayer(), OptionsMenuClass);

		// Adding a Z-Order of 101 ensures this menu renders ON TOP of the Pause Menu
		if (OptionsMenu) OptionsMenu->AddToViewport(101);
	}
}

void UPauseMenuWidget::OnQuitClicked()
{
	// Native Game Instance fetch via the World context
	if (UWorld* World = GetWorld())
	{
		UProjGameInst* GameInst = Cast<UProjGameInst>(World->GetGameInstance());
		if (GameInst)
		{
			// Remove the pause state before traveling to prevent frozen loads
			APlayerController* PC = GetOwningPlayer();
			if (PC) PC->SetPause(false);

			// Execute the transition to Index 0 (CodeMenuMap)
			GameInst->LoadLevelSafe(0);
		}
	}
}
