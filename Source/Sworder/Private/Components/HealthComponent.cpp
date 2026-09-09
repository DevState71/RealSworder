// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
#include "Components/TagManager.h"
#include "GameplayTagContainer.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

float UHealthComponent::ProcessDamageType_Implementation(float DamageAmount, AActor* DamageCauser)
{
	UTagManager* SelfTagManager = GetOwner()->FindComponentByClass<UTagManager>();

	UTagManager* AttackerTagManager = DamageCauser->FindComponentByClass<UTagManager>();

	if(SelfTagManager && AttackerTagManager)
	{
		const FGameplayTagContainer& SelfTags = SelfTagManager->GetGameplayTagContainer();
		const FGameplayTagContainer& AttackerTags = AttackerTagManager->GetGameplayTagContainer();

		if(AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Fire")) && SelfTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Ice")))
		{
			DamageAmount *= 2.0f; // Fire is strong against Ice
			UE_LOG(LogTemp, Warning, TEXT("Fire is strong against Ice! Damage doubled."));
		}
		else if(AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Ice")) && SelfTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Fire")))
		{
			DamageAmount *= 0.5f; // Ice is weak against Fire
			UE_LOG(LogTemp, Warning, TEXT("Ice is weak against Fire! Damage halved."));
		}

	}
	
	return DamageAmount;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

