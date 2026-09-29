// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboDataAsset.h"
#include "ComboJournalWidget.generated.h"

class UScrollBox;
class UComboRowWidget;

UCLASS()
class SWORDER_API UComboJournalWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	// The listener function
	UFUNCTION()
	void HandleNewComboDiscovered(FElementCombo DiscoveredCombo);

protected:
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ComboScrollBox;

	// The specific Blueprint Row class to spawn for each new combo
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UComboRowWidget> ComboRowClass;
};
