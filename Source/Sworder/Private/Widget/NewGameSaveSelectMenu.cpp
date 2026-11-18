// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NewGameSaveSelectMenu.h"

#include "GAFretter/Instances/ProjGameInst.h"

void UNewGameSaveSelectMenu::HandleSaveSlotClicked(FString SaveSlotName)
{

	if (UProjGameInst* GameInst = Cast<UProjGameInst>(GetGameInstance())) {
		GameInst->GameLoaded.AddDynamic(GameInst, &UProjGameInst::LoadFirstLevel);
		GameInst->NewSave(SaveSlotName);
	}

}
