// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BaseSaveSlot.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

void UBaseSaveSlot::NativePreConstruct()
{

}

void UBaseSaveSlot::NativeConstruct()
{
	if (SaveSlotButton)
		SaveSlotButton->OnClicked.AddUniqueDynamic(this, &UBaseSaveSlot::HandleSaveButtonClick);
}

void UBaseSaveSlot::HandleSaveButtonClick()
{
	SaveSlotButtonClicked.Broadcast();
}

void UBaseSaveSlot::SetSaveSlotName(const FText NewSaveSlotName)
{
	if (SaveSlotNameText)
		SaveSlotNameText->SetText(NewSaveSlotName);
}

void UBaseSaveSlot::SetSaveDataName(const FText NewDataName)
{
	if (SaveData_Name)
		SaveData_Name->SetText(NewDataName);
}

void UBaseSaveSlot::SetSaveDataValue(const FText NewValue)
{
	if (SaveData_Value)
		SaveData_Value->SetText(NewValue);
}
