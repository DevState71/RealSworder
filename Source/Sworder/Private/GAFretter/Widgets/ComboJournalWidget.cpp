// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/Widgets/ComboJournalWidget.h"
#include "GAFretter/Widgets/ComboRowWidget.h"
#include "Components/HealthComponent.h"
#include "Components/ScrollBox.h"

void UComboJournalWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind to the player's Health Component natively
	if (APawn* PlayerPawn = GetOwningPlayerPawn())
	{
		if (UHealthComponent* HealthComp = PlayerPawn->FindComponentByClass<UHealthComponent>())
		{
			HealthComp->OnComboDiscovered.AddDynamic(this, &UComboJournalWidget::HandleNewComboDiscovered);
		}
	}
}

void UComboJournalWidget::HandleNewComboDiscovered(FElementCombo DiscoveredCombo)
{
	if (ComboRowClass && ComboScrollBox)
	{
		// Spawn a new row, initialize the text, and inject it into the Scroll Box
		if (UComboRowWidget* NewRow = CreateWidget<UComboRowWidget>(GetWorld(), ComboRowClass))
		{
			NewRow->InitializeRow(DiscoveredCombo);
			ComboScrollBox->AddChild(NewRow);
		}
	}
}
