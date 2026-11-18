// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/SaveSelectMenu.h"
#include "NewGameSaveSelectMenu.generated.h"

/**
 * 
 */
UCLASS()
class SWORDER_API UNewGameSaveSelectMenu : public USaveSelectMenu
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;

	virtual void HandleSaveSlotClicked(FString SaveSlotName) override;

	virtual void SetUpSaveSlot(class UBaseSaveSlot* SaveSlot) override;
};
