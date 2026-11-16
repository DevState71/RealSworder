// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseSaveSlot.generated.h"

/**
 * 
 */
UCLASS()
class SWORDER_API UBaseSaveSlot : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* SaveSlotButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* SaveSlotNameText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* SaveData_Name;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* SaveData_Value;

public:
	void SetSaveSlotName(const FText NewSaveSlotName);
	void SetSaveDataName(const FText NewDataName);
	void SetSaveDataValue(const FText NewValue);
};
