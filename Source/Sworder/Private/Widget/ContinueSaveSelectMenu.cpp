// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ContinueSaveSelectMenu.h"

#include "Widget/BaseSaveSlot.h"
#include "GAFretter/Instances/ProjGameInst.h"



void UContinueSaveSelectMenu::NativeConstruct()
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

void UContinueSaveSelectMenu::HandleSaveSlotClicked(FString SaveSlotName)
{
	if (UProjGameInst* GameInst = Cast<UProjGameInst>(GetGameInstance())) {
		GameInst->GameLoaded.AddDynamic(GameInst, &UProjGameInst::LoadFirstLevel);
		GameInst->LoadGame(SaveSlotName);
	}

}

void UContinueSaveSelectMenu::SetUpSaveSlot(UBaseSaveSlot* SaveSlot)
{
	if (GetSaveFileInfo(SaveSlot, SaveSlot->GetSaveSlotName()))
		SaveSlot->SaveSlotButtonClicked.AddUniqueDynamic(this, &UContinueSaveSelectMenu::HandleSaveSlotClicked);
	else
		SaveSlot->SetButtonIsEnabled(false);
}
