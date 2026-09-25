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

void UHealthComponent::TakeFlatDamage_Implementation(float DamageAmount, AActor* DamageCauser)
{

}

float UHealthComponent::ProcessDamageType_Implementation(float DamageAmount, AActor* DamageCauser)
{
	UTagManager* SelfTagManager = GetOwner()->FindComponentByClass<UTagManager>();

	UTagManager* AttackerTagManager = DamageCauser->FindComponentByClass<UTagManager>();

	FGameplayTagContainer SelfTags;
	FGameplayTagContainer AttackerTags;

	if(SelfTagManager)
	{
		SelfTags = SelfTagManager->GetGameplayTagContainer();
	}

	if(AttackerTagManager)
	{
		AttackerTags = AttackerTagManager->GetGameplayTagContainer();
	}

	if (SelfTagManager && AttackerTagManager)
	{
		


		if (SelfTags.HasTag(FGameplayTag::RequestGameplayTag("Element")) && AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element")))
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

			if (AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Fire")) && SelfTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Earth")))
			{
				DamageAmount *= 2.0f; // Fire is strong against Earth
				UE_LOG(LogTemp, Warning, TEXT("Fire is strong against Earth! Damage doubled."));
			}
			else if (AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Earth")) && SelfTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Fire")))
			{
				DamageAmount *= 0.5f; // Earth is weak against Fire
				UE_LOG(LogTemp, Warning, TEXT("Earth is weak against Fire! Damage halved."));
			}

			if (AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Earth")) && SelfTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Lightning")))
			{
				DamageAmount *= 2.0f; // Earth is strong against Lightning
				UE_LOG(LogTemp, Warning, TEXT("Earth is strong against Lightning! Damage doubled."));
			}
			else if (AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Lightning")) && SelfTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Earth")))
			{
				DamageAmount *= 0.5f; // Lightning is weak against Earth
				UE_LOG(LogTemp, Warning, TEXT("Lightning is weak against Earth! Damage halved."));
			}

			if (AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Lightning")) && SelfTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Ice")))
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

/*
		if (AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Fire")))
		{
			SelfTagManager->StatusRoll(FGameplayTag::RequestGameplayTag("Element.Fire"));
		}
		else if (AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Ice")))
		{
			SelfTagManager->StatusRoll(FGameplayTag::RequestGameplayTag("Element.Ice"));
		}
		else if (AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Lightning")))
		{
			SelfTagManager->StatusRoll(FGameplayTag::RequestGameplayTag("Element.Lightning"));
		}

	}
	if (AttackerTagManager)
	{
		if (AttackerTags.HasTag(FGameplayTag::RequestGameplayTag("Element.Earth")))
		{
			UHealthComponent* AttackerHealthComponent = DamageCauser->FindComponentByClass<UHealthComponent>();

			if (AttackerHealthComponent)
			{
				AttackerTagManager->StatusRoll(FGameplayTag::RequestGameplayTag("Element.Earth"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Attacker does not have a HealthComponent, checking the owning Actor."));

				AActor* AttackerActor = Cast<AActor>(DamageCauser->GetOwner());

				if (AttackerActor)
				{
					AttackerHealthComponent = AttackerActor->FindComponentByClass<UHealthComponent>();

					if (AttackerHealthComponent)
					{
						UTagManager* AttackerOwnerTagManager = AttackerActor->FindComponentByClass<UTagManager>();

						if (AttackerOwnerTagManager)
						{
							AttackerOwnerTagManager->StatusRoll(FGameplayTag::RequestGameplayTag("Element.Earth"));
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Attacker's owner does not have a TagManager."));
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Attacker's owner does not have a HealthComponent."));
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("DamageCauser is not owned by an Actor."));
				}

			}

		}*/
	} 

	if (AttackerTagManager)
	{
		FElementCombo* Combo = AttackerTagManager->ComboBuild(AttackerTags.GetByIndex(0));

		if(Combo == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("No combo triggered."));
		}
		else
		{
			if (Combo->bEndsCombo)
			{
				AttackerTagManager->EndCombo();
			}

			if (Combo->SelfOrEnemy)
			{
				SelfTagManager->StatusRoll(Combo->Result);
			}
			else
			{
				AttackerTagManager->StatusRoll(Combo->Result);
			}
		}
	}


	
	return DamageAmount;
}


float UHealthComponent::DamageShield_Implementation(float DamageAmount, AActor* DamageCauser)
{
	if(ShieldHP > 0.0f)
	{
		if(DamageAmount <= ShieldHP)
		{
			
			ShieldHP -= DamageAmount;
			UE_LOG(LogTemp, Warning, TEXT("Shield absorbed all damage, remaining HP: %f"), ShieldHP);
			DamageAmount = 0.0f;
		}
		else
		{
			DamageAmount -= ShieldHP;
			UE_LOG(LogTemp, Warning, TEXT("Shield absorbed some damage, remaining damage: %f"), DamageAmount);
			ShieldHP = 0.0f;
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

