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
	int32 RandomNumber = FMath::RandRange(1, 100);

	if (RandomNumber <= 70)
	{
		if (tag == FGameplayTag::RequestGameplayTag("Element.Fire"))
		{
			AddGameplayTag(FGameplayTag::RequestGameplayTag("Status.Burn"));
		}
		else if( tag == FGameplayTag::RequestGameplayTag("Element.Ice"))
		{
			AddGameplayTag(FGameplayTag::RequestGameplayTag("Status.Slow"));
		}
		else if(tag == FGameplayTag::RequestGameplayTag("Element.Earth"))
		{
			AddGameplayTag(FGameplayTag::RequestGameplayTag("Status.Shield"));
		}
		else if(tag == FGameplayTag::RequestGameplayTag("Element.Lightning"))
		{
			AddGameplayTag(FGameplayTag::RequestGameplayTag("Status.Stun"));
		}
	

	}
}

void UTagManager::AddGameplayTag(FGameplayTag tag)
{
	if(!GameplayTagContainer.HasTag(tag))
	{
		GameplayTagContainer.AddTag(tag);

		AddStatusTag.Broadcast(tag, true);
	}
}

