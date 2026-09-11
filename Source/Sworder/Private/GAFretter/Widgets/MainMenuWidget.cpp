// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/Widgets/MainMenuWidget.h"
#include "GAFretter/Widgets/ButtonWithText.h"
#include "GAFretter/Instances/ProjGameInst.h"
#include "Sound/SoundBase.h"

void UMainMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (MenuMusic) PlaySound(MenuMusic);

	if (PlayButton) PlayButton->SetButtonText(FText::FromString("Play"));
	if (QuitButton) QuitButton->SetButtonText(FText::FromString("Quit"));
	if (OptionsButton) OptionsButton->SetButtonText(FText::FromString("Options"));
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind the Play Button and Quit Button to our Game Instance transition logic
	if (PlayButton) PlayButton->OnButtonClicked.AddDynamic(this, &UMainMenuWidget::OnPlayClicked);
	if (QuitButton) QuitButton->OnButtonClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
	if (OptionsButton) OptionsButton->OnButtonClicked.AddDynamic(this, &UMainMenuWidget::OnOptionsClicked);
}

	// Get our custom Game Instance to load the first level
void UMainMenuWidget::OnPlayClicked() { if (UProjGameInst* GameInst = Cast<UProjGameInst>(GetGameInstance())) GameInst->LoadFirstLevel(); }

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
