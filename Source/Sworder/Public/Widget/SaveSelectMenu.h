// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveSelectMenu.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWidgetDelegate);

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

	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* BackButton;

	virtual void SetUpSaveSlot(class UBaseSaveSlot* SaveSlot);

	bool GetSaveFileInfo(class UBaseSaveSlot* SaveSlot, FString SaveSlotName);

	UFUNCTION()
	virtual void HandleSaveSlotClicked(FString SaveSlotName);

	UFUNCTION()
	void OnBackClicked();

public:
	FWidgetDelegate WidgetRemoved;

};
