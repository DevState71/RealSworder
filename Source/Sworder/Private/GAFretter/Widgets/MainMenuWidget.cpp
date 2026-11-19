// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/Widgets/MainMenuWidget.h"
#include "GAFretter/Widgets/ButtonWithText.h"
#include "GAFretter/Instances/ProjGameInst.h"
#include "Sound/SoundBase.h"

void UMainMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (MenuMusic) PlaySound(MenuMusic);

	if (ContinueButton) ContinueButton->SetButtonText(FText::FromString("Continue"));
	if (PlayButton) PlayButton->SetButtonText(FText::FromString("New Game"));
	if (QuitButton) QuitButton->SetButtonText(FText::FromString("Quit"));
	if (OptionsButton) OptionsButton->SetButtonText(FText::FromString("Options"));
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind the Play Button and Quit Button to our Game Instance transition logic
	if (ContinueButton) ContinueButton->OnButtonClicked.AddDynamic(this, &UMainMenuWidget::OnContinueClicked);
	if (PlayButton) PlayButton->OnButtonClicked.AddDynamic(this, &UMainMenuWidget::OnPlayClicked);
	if (QuitButton) QuitButton->OnButtonClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
	if (OptionsButton) OptionsButton->OnButtonClicked.AddDynamic(this, &UMainMenuWidget::OnOptionsClicked);
}


void UMainMenuWidget::ShowSubMenu(UUserWidget* Widget)
{
	if (Widget) {
		Widget->AddToViewport(101);
		ToggleMenuButtonVisibility(false);
	}
}

void UMainMenuWidget::OnSubMenuRemoved()
{
	ToggleMenuButtonVisibility(true);
}

void UMainMenuWidget::OnContinueClicked()
{
	if (ContinueGameMenuClass) {
		USaveSelectMenu* ContinueGameMenu = CreateWidget<USaveSelectMenu>(GetOwningPlayer(), ContinueGameMenuClass);
		if (ContinueGameMenu) {
			//ContinueGameMenu->AddToViewport(101);
			ShowSubMenu(ContinueGameMenu);
			ContinueGameMenu->WidgetRemoved.AddUniqueDynamic(this, &UMainMenuWidget::OnSubMenuRemoved);

		}
	}
}

// Get our custom Game Instance to load the first level
void UMainMenuWidget::OnPlayClicked() 
{ 
	if (NewGameMenuClass) {
		USaveSelectMenu* NewGameMenu = CreateWidget<USaveSelectMenu>(GetOwningPlayer(), NewGameMenuClass);
		if (NewGameMenu) {
			//NewGameMenu->AddToViewport(101);
			ShowSubMenu(NewGameMenu);
			NewGameMenu->WidgetRemoved.AddUniqueDynamic(this, &UMainMenuWidget::OnSubMenuRemoved);
		}
	}
}

void UMainMenuWidget::OnQuitClicked() { if (UProjGameInst* GameInst = Cast<UProjGameInst>(GetGameInstance())) GameInst->QuitTheGame(); }

void UMainMenuWidget::OnOptionsClicked()
{
	if (OptionsMenuClass)
	{
		// Construct and display the options menu
		UUserWidget* OptionsMenu = CreateWidget<UUserWidget>(GetOwningPlayer(), OptionsMenuClass);
		if (OptionsMenu)
		{
			// Using 101 ensures it renders completely on top of the Main Menu
			OptionsMenu->AddToViewport(101);
		}
	}
}
