// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/Widgets/ButtonWithText.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Sound/SoundBase.h"

void UButtonWithText::SetButtonText(FText NewText)
{
    if (ButtonText) ButtonText->SetText(NewText);
}

bool UButtonWithText::Initialize()
{
    if (!Super::Initialize()) return false;

    // Always null-check bound widgets before using them
    if (MainButton)
    {
        // Bind the Click event
        MainButton->OnClicked.RemoveDynamic(this, &UButtonWithText::HandleButtonClicked);
        MainButton->OnClicked.AddDynamic(this, &UButtonWithText::HandleButtonClicked);

        // Bind the Hover event
        MainButton->OnHovered.RemoveDynamic(this, &UButtonWithText::HandleButtonHovered);
        MainButton->OnHovered.AddDynamic(this, &UButtonWithText::HandleButtonHovered);
    }

    return true;
}

void UButtonWithText::HandleButtonClicked()
{
    // Play the click sound purely through native UI code
    if (ClickSound) PlaySound(ClickSound);
    
    // Check if anyone (like our MainMenu) is actually listening, then broadcast the event to the listeners, if so.
    if (OnButtonClicked.IsBound()) OnButtonClicked.Broadcast();
}

void UButtonWithText::HandleButtonHovered()
{
    // Play the hover sound purely through native UI code
	if (HoverSound) PlaySound(HoverSound);
}
