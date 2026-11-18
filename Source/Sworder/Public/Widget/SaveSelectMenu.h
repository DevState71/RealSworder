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


	bool GetSaveFileInfo(class UBaseSaveSlot* SaveSlot, FString SaveSlotName);

	UFUNCTION()
	void OnSaveSlot_1Clicked();

	UFUNCTION()
	void OnSaveSlot_2Clicked();

	
	virtual void HandleSaveSlotClicked(FString SaveSlotName);
};
