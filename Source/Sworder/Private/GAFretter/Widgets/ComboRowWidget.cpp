// Fill out your copyright notice in the Description page of Project Settings.


#include "GAFretter/Widgets/ComboRowWidget.h"
#include "Components/TextBlock.h"

void UComboRowWidget::InitializeRow(const FElementCombo& ComboData)
{
	StoredCombo = ComboData;

	if (ComboText)
	{
		FString FormulaString = "";

		for (int32 i = 0; i < ComboData.ComboSequence.Num(); ++i)
		{
			// Splits "Element.Fire" at the period and keeps only "Fire"
			FString TagName = ComboData.ComboSequence[i].GetTagName().ToString();
			TagName.Split(TEXT("."), nullptr, &TagName);

			FormulaString += TagName;
			if (i < ComboData.ComboSequence.Num() - 1)
			{
				FormulaString += " + ";
			}
		}

		// Splits "Status.Ember" and keeps only "Ember"
		FString ResultName = ComboData.Result.GetTagName().ToString();
		ResultName.Split(TEXT("."), nullptr, &ResultName);

		FormulaString += " = " + ResultName;

		// Output Example: "Fire + Fire = Ember"
		ComboText->SetText(FText::FromString(FormulaString));
	}
}