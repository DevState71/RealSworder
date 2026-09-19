// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatusComponent.h"
#include "Components/TagManager.h"
#include "Components/HealthComponent.h"

// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	UTagManager* TagManager = GetOwner()->FindComponentByClass<UTagManager>();

	if(TagManager)
	{
		TagManager->AddStatusTag.AddDynamic(this, &UStatusComponent::HandleStatusTag);
	}
	
}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatusComponent::HandleStatusTag(FGameplayTag tag, bool bAdded)
{
	if (tag == FGameplayTag::RequestGameplayTag("Status.Burn"))
	{
		if(bAdded)
		{
			AddBurn();
		}
		else
		{
			RemoveBurn();
		}
	}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Slow"))
	{
		if(bAdded)
		{
			AddSlow();
		}
		else
		{
			RemoveSlow();
		}
	}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Stun"))
	{
		if(bAdded)
		{
			AddStun();
		}
		else
		{
			RemoveStun();
		}
	}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Shield"))
	{
		if (bAdded)
		{
			AddShield();
		}
		else
		{
			RemoveShield();
		}
	}
}

void UStatusComponent::AddBurn()
{
	//Burn stuff
	UE_LOG(LogTemp, Warning, TEXT("Burning!"));
	GetWorld()->GetTimerManager().SetTimer(BurnTimerHandle, this, &UStatusComponent::RemoveBurn, BurnTimer, false);

	GetWorld()->GetTimerManager().SetTimer(BurnDamageTickHandle, this, &UStatusComponent::BurnDamageTickFunction, BurnDamageTickInterval, true);
}

void UStatusComponent::RemoveBurn()
{
	
	GetWorld()->GetTimerManager().ClearTimer(BurnTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(BurnDamageTickHandle);
	UE_LOG(LogTemp, Warning, TEXT("Burn removed!"));
	UTagManager* TagManager = GetOwner()->FindComponentByClass<UTagManager>();
	if(TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Burn")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Burn"));
	}
}

void UStatusComponent::BurnDamageTickFunction()
{
	UE_LOG(LogTemp, Warning, TEXT("Ow!"));
	UHealthComponent* HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
	if(HealthComponent)
	{
		HealthComponent->TakeFlatDamage(BurnDamageTick, nullptr);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No HealthComponent found on owner!"));
	}

	GetWorld()->GetTimerManager().SetTimer(BurnDamageTickHandle, this, &UStatusComponent::BurnDamageTickFunction, BurnDamageTickInterval, true);
}

void UStatusComponent::AddSlow()
{
	AddStatusTag.Broadcast(FGameplayTag::RequestGameplayTag("Status.Slow"), true);
	UE_LOG(LogTemp, Warning, TEXT("Slowed!"));
	GetWorld()->GetTimerManager().SetTimer(SlowTimerHandle, this, &UStatusComponent::RemoveSlow, SlowTimer, false);
}

void UStatusComponent::RemoveSlow()
{
	AddStatusTag.Broadcast(FGameplayTag::RequestGameplayTag("Status.Slow"), false);
	GetWorld()->GetTimerManager().ClearTimer(SlowTimerHandle);
	UE_LOG(LogTemp, Warning, TEXT("Slow removed!"));
	UTagManager* TagManager = GetOwner()->FindComponentByClass<UTagManager>();
	if(TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Slow")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Slow"));
	}
}

void UStatusComponent::AddStun()
{
	AddStatusTag.Broadcast(FGameplayTag::RequestGameplayTag("Status.Stun"), true);
	UE_LOG(LogTemp, Warning, TEXT("Stunned!"));
	GetWorld()->GetTimerManager().SetTimer(StunTimerHandle, this, &UStatusComponent::RemoveStun, StunTimer, false);
}

void UStatusComponent::RemoveStun()
{
	AddStatusTag.Broadcast(FGameplayTag::RequestGameplayTag("Status.Stun"), false);
	GetWorld()->GetTimerManager().ClearTimer(StunTimerHandle);
	UE_LOG(LogTemp, Warning, TEXT("Stun removed!"));
	UTagManager* TagManager = GetOwner()->FindComponentByClass<UTagManager>();
	if(TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Stun")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Stun"));
	}
}

void UStatusComponent::AddShield()
{
	UE_LOG(LogTemp, Warning, TEXT("Shielded!"));
	UHealthComponent* HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
	if(HealthComponent)
	{
		HealthComponent->ShieldHP = ShieldAmount; // Set shield HP to ShieldAmount
		UE_LOG(LogTemp, Warning, TEXT("Shield HP set to: %f"), HealthComponent->ShieldHP);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No HealthComponent found on owner!"));
	}


	GetWorld()->GetTimerManager().SetTimer(ShieldTimerHandle, this, &UStatusComponent::RemoveShield, ShieldTimer, false);
}

void UStatusComponent::RemoveShield()
{
	UHealthComponent* HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
	if(HealthComponent)
	{
		HealthComponent->ShieldHP = 0.0f; // Reset shield HP to 0
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No HealthComponent found on owner!"));
	}
	GetWorld()->GetTimerManager().ClearTimer(ShieldTimerHandle);
	UE_LOG(LogTemp, Warning, TEXT("Shield removed!"));
	UTagManager* TagManager = GetOwner()->FindComponentByClass<UTagManager>();
	if(TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Shield")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Shield"));
	}
}


