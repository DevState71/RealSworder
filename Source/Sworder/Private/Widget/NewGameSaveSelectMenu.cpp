// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NewGameSaveSelectMenu.h"

#include "Widget/BaseSaveSlot.h"
#include "GAFretter/Instances/ProjGameInst.h"



void UNewGameSaveSelectMenu::NativeConstruct()
{
	if (SaveSlot_1)
	{
		SetUpSaveSlot(SaveSlot_1);
	}

	if (SaveSlot_2)
	{
		SetUpSaveSlot(SaveSlot_2);
	}
}

void UNewGameSaveSelectMenu::HandleSaveSlotClicked(FString SaveSlotName)
{

	if (UProjGameInst* GameInst = Cast<UProjGameInst>(GetGameInstance())) {
		GameInst->GameLoaded.AddDynamic(GameInst, &UProjGameInst::LoadFirstLevel);
		GameInst->NewSave(SaveSlotName);
	}

}

void UNewGameSaveSelectMenu::SetUpSaveSlot(UBaseSaveSlot* SaveSlot)
{
	GetSaveFileInfo(SaveSlot, SaveSlot->GetSaveSlotName());
	SaveSlot->SaveSlotButtonClicked.AddUniqueDynamic(this, &UNewGameSaveSelectMenu::HandleSaveSlotClicked);
}
