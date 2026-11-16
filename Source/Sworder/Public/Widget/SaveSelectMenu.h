// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveSelectMenu.generated.h"

/**
 * 
 */
UCLASS()
class SWORDER_API USaveSelectMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UBaseSaveSlot* SaveSlot_1;
	UPROPERTY(meta = (BindWidget))
	class UBaseSaveSlot* SaveSlot_2;


	void GetSaveFileInfo(class UBaseSaveSlot* SaveSlot, FString SaveSlotName);
};
