// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonWithText.generated.h"

class UButton;
class UTextBlock;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomButtonDelegate);

/**
 * 
 */
UCLASS()
class SWORDER_API UButtonWithText : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void SetButtonText(FText NewText);

        UPROPERTY(BlueprintAssignable, Category = "Events")
    FCustomButtonDelegate OnButtonClicked;

        UFUNCTION(BlueprintCallable, Category = "Button Actions")
    void HandleButtonClicked();

protected:
    virtual bool Initialize() override;

        UPROPERTY(meta = (BindWidget))
    UButton* MainButton;

        UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
    UTextBlock* ButtonText;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
    USoundBase* HoverSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
    USoundBase* ClickSound;

private:
    UFUNCTION()
    void HandleButtonHovered();
};
