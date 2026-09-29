// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TagManager.h"

// Sets default values for this component's properties
UTagManager::UTagManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTagManager::BeginPlay()
{
	Super::BeginPlay();

	if(!ComboDataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("ComboDataAsset is not assigned in TagManager on %s"), *GetOwner()->GetName());
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Loaded ComboDataAsset with %d combos"), ComboDataAsset->StoredCombos.Num());
	
	// ...
	
}


// Called every frame
void UTagManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTagManager::StatusRoll(FGameplayTag tag)
{
	UE_LOG(LogTemp, Warning, TEXT("StatusRoll called with tag: %s"), *tag.ToString());

	int32 RandomNumber = FMath::RandRange(1, 100);

	if (RandomNumber <= StatusChance)
	{
		AddGameplayTag(tag);
		UE_LOG(LogTemp, Warning, TEXT("StatusRoll succeeded with tag: %s"), *tag.ToString());
	

	}
}

FElementCombo* UTagManager::ComboBuild(FGameplayTag Tag)
{
	

	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);

	ComboTags.Add(Tag);
	CurrentComboTag = Tag;
	FElementCombo* Result = TriggerCombo();


	GetWorld()->GetTimerManager().SetTimer(
		ComboTimerHandle,
		this,
		&UTagManager::EndCombo,
		ComboTime,
		false
	);

	return Result;
}

FElementCombo* UTagManager::TriggerCombo()
{
	FElementCombo* Combo = nullptr;

	bool neutral = CheckNeutral();
	if(neutral)
	{
		UE_LOG(LogTemp, Warning, TEXT("Neutral tag detected, ending combo."));
		return nullptr;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("TriggerCombo called with %d combo tags"), ComboTags.Num());

for(FElementCombo& StoredCombo : ComboDataAsset->StoredCombos)
	{
		if(StoredCombo.ComboSequence == this->ComboTags)
		{
			Combo = &StoredCombo;
			break;
		}
}

	return Combo;
}

bool UTagManager::CheckNeutral()
{
	

	if(CurrentComboTag == FGameplayTag::RequestGameplayTag("Element.Neutral"))
	{
		int32 coinFlip = FMath::RandRange(0, 1);
		ComboTags.Pop();

		if(coinFlip == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Neutral tag detected, ending combo."));
			EndCombo();
			return true;
		}

	}

	
	return false;
}
void UTagManager::EndCombo()
{
	ComboTags.Empty();
	UE_LOG(LogTemp, Warning, TEXT("Combo ended."));
}

void UTagManager::AddGameplayTag(FGameplayTag tag)
{
	if(!GameplayTagContainer.HasTag(tag))
	{
		GameplayTagContainer.AddTag(tag);

		AddStatusTag.Broadcast(tag, true);
	}
}

