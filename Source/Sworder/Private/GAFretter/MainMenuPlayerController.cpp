// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/MainMenuPlayerController.h"
#include "Blueprint/UserWidget.h"

AMainMenuPlayerController::AMainMenuPlayerController()
{
	// Set this player controller to not show the mouse cursor by default
	bShowMouseCursor = true;
}

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (MenuClass)
	{
		// "Create User Widget Widget"
		UUserWidget* MenuWidget = CreateWidget<UUserWidget>(this, MenuClass);

		if (MenuWidget)
		{
			// "Add to Viewport"
			MenuWidget->AddToViewport();

			// "Set Input Mode UI Only"
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(MenuWidget->TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);

			SetInputMode(InputModeData);

			// "Flush Input"
			FlushPressedKeys();
		}
	}

}
