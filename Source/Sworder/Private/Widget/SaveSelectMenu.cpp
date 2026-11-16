// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SaveSelectMenu.h"

#include "Widget/BaseSaveSlot.h"
#include "GAFretter/Instances/ProjGameInst.h"
//#include "Kismet/GameplayStatics.h"


void USaveSelectMenu::NativePreConstruct()
{
	if (SaveSlot_1) {
		SaveSlot_1->SetSaveSlotName(FText::FromString("Save Slot 1"));
		SaveSlot_1->SetSaveDataName(FText::FromString("Gold: "));
		SaveSlot_1->SetSaveDataValue(FText::FromString("0000"));
	}

	if (SaveSlot_2) {
		SaveSlot_2->SetSaveSlotName(FText::FromString("Save Slot 2"));
		SaveSlot_2->SetSaveDataName(FText::FromString("Gold: "));
		SaveSlot_2->SetSaveDataValue(FText::FromString("0000"));
	}
}

void USaveSelectMenu::NativeConstruct()
{
	if (SaveSlot_1)
	{
		// Check for an existing save and set the value. If none exist set name to "New Save" and Data to be empty.
		if (GetSaveFileInfo(SaveSlot_1, "SaveSlot_1")) {
			// Should only be called if save file found
			SaveSlot_1->SaveSlotButtonClicked.AddUniqueDynamic(this, &USaveSelectMenu::OnSaveSlot_1Clicked);
		}
	}

	if (SaveSlot_2)
	{
		// Check for an existing save and set the value. If none exist set name to "New Save" and Data to be empty.
		if (GetSaveFileInfo(SaveSlot_2, "SaveSlot_2"))
		{
			SaveSlot_2->SaveSlotButtonClicked.AddUniqueDynamic(this, &USaveSelectMenu::OnSaveSlot_2Clicked);
		}
	}
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
			SaveSlot->SetSaveSlotName(FText::FromString("Empty"));
			SaveSlot->SetSaveDataName(FText::FromString(""));
			SaveSlot->SetSaveDataValue(FText::FromString(""));
		}
	}

	return false;
}

void USaveSelectMenu::OnSaveSlot_1Clicked()
{
	HandleSaveSlotClicked("SaveSlot_1");
}

void USaveSelectMenu::OnSaveSlot_2Clicked()
{
	HandleSaveSlotClicked("SaveSlot_2");
}

void USaveSelectMenu::HandleSaveSlotClicked(FString SaveSlotName)
{
	if (UProjGameInst* GameInst = Cast<UProjGameInst>(GetGameInstance())) {
		GameInst->GameLoaded.AddDynamic(GameInst, &UProjGameInst::LoadFirstLevel);
		GameInst->LoadGame(SaveSlotName);
	}
}

