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

		if(SelfTags.HasTag(FGameplayTag::RequestGameplayTag("Element")) && AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Both the attacker and the target have Element tags!"));

			if (AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Ice")) && SelfTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Fire")))
			{
				DamageAmount *= 2.0f; // Ice is strong against Fire
				UE_LOG(LogTemp, Warning, TEXT("Ice is strong against Fire! Damage doubled."));
			}
			else if (AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Fire")) && SelfTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Ice")))
			{
				DamageAmount *= 0.5f; // Fire is weak against Ice
				UE_LOG(LogTemp, Warning, TEXT("Fire is weak against Ice! Damage halved."));
			}

			if(AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Fire")) && SelfTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Earth")))
			{
				DamageAmount *= 2.0f; // Fire is strong against Earth
				UE_LOG(LogTemp, Warning, TEXT("Fire is strong against Earth! Damage doubled."));
			}
			else if (AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Earth")) && SelfTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Fire")))
			{
				DamageAmount *= 0.5f; // Earth is weak against Fire
				UE_LOG(LogTemp, Warning, TEXT("Earth is weak against Fire! Damage halved."));
			}

			if(AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Earth")) && SelfTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Lightning")))
			{
				DamageAmount *= 2.0f; // Earth is strong against Lightning
				UE_LOG(LogTemp, Warning, TEXT("Earth is strong against Lightning! Damage doubled."));
			}
			else if (AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Lightning")) && SelfTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Earth")))
			{
				DamageAmount *= 0.5f; // Lightning is weak against Earth
				UE_LOG(LogTemp, Warning, TEXT("Lightning is weak against Earth! Damage halved."));
			}

			if(AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Lightning")) && SelfTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Ice")))
			{
				DamageAmount *= 2.0f; // Lightning is strong against Ice
				UE_LOG(LogTemp, Warning, TEXT("Lightning is strong against Ice! Damage doubled."));
			}
			else if (AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Ice")) && SelfTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Lightning")))
			{
				DamageAmount *= 0.5f; // Ice is weak against Lightning
				UE_LOG(LogTemp, Warning, TEXT("Ice is weak against Lightning! Damage halved."));
			}

			

		
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

