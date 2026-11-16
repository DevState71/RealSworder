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
		GetSaveFileInfo(SaveSlot_1, "SaveSlot_1");
	}

	if (SaveSlot_2)
	{
		// Check for an existing save and set the value. If none exist set name to "New Save" and Data to be empty.
	}
}

void USaveSelectMenu::GetSaveFileInfo(class UBaseSaveSlot* SaveSlot, FString SaveSlotName)
{
	if (UProjGameInst* GameInst = Cast<UProjGameInst>(GetGameInstance())) {
		SaveSlot->SetSaveDataValue(FText::AsNumber(GameInst->LoadSaveFileInfo("SaveSlot_1")));

	}
}

