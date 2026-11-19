// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SaveSelectMenu.h"

#include "Widget/BaseSaveSlot.h"
#include "GAFretter/Instances/ProjGameInst.h"
#include "GAFretter/Widgets/ButtonWithText.h"


void USaveSelectMenu::NativePreConstruct()
{
	if (BackButton) {
		BackButton->SetButtonText(FText::FromString("Back"));
	}

	if (SaveSlot_1) {
		SaveSlot_1->SetSaveSlotNameText(FText::FromString("Save Slot 1"));
		SaveSlot_1->SetSaveSlotName("Save Slot 1");
		SaveSlot_1->SetSaveDataName(FText::FromString("Gold: "));
		SaveSlot_1->SetSaveDataValue(FText::FromString("0000"));
	}

	if (SaveSlot_2) {
		SaveSlot_2->SetSaveSlotNameText(FText::FromString("Save Slot 2"));
		SaveSlot_2->SetSaveSlotName("Save Slot 2");
		SaveSlot_2->SetSaveDataName(FText::FromString("Gold: "));
		SaveSlot_2->SetSaveDataValue(FText::FromString("0000"));
	}
}

void USaveSelectMenu::NativeConstruct()
{
	if (BackButton)
		BackButton->OnButtonClicked.AddUniqueDynamic(this, &USaveSelectMenu::OnBackClicked);
}

void USaveSelectMenu::SetUpSaveSlot(class UBaseSaveSlot* SaveSlot)
{
	// Defined in child classes
}

bool USaveSelectMenu::GetSaveFileInfo(class UBaseSaveSlot* SaveSlot, FString SaveSlotName)
{
	if (UProjGameInst* GameInst = Cast<UProjGameInst>(GetGameInstance())) {

		int SaveData = 0;
		if (GameInst->LoadSaveFileInfo(SaveSlotName, SaveData)) {
			SaveSlot->SetSaveDataValue(FText::AsNumber(SaveData));
			return true;
		}
		else {
			SaveSlot->SetSaveSlotNameText(FText::FromString("Empty"));
			SaveSlot->SetSaveDataName(FText::FromString(""));
			SaveSlot->SetSaveDataValue(FText::FromString(""));
		}
	}

	return false;
}

void USaveSelectMenu::HandleSaveSlotClicked(FString SaveSlotName)
{
	// Defined in child classes
}

void USaveSelectMenu::OnBackClicked()
{
	WidgetRemoved.Broadcast();
	this->RemoveFromParent();
}
