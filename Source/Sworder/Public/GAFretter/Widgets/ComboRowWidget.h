// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboDataAsset.h"
#include "ComboRowWidget.generated.h"

class UTextBlock;

UCLASS()
class SWORDER_API UComboRowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Sets up the text dynamically when spawned
	void InitializeRow(const FElementCombo& ComboData);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ComboText;

	// Exposes the combo to Blueprints so you can read the Tooltip data when hovering
	UPROPERTY(BlueprintReadOnly, Category = "Combo Data")
	FElementCombo StoredCombo;
};
