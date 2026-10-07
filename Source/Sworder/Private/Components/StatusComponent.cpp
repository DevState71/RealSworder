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

	if (tag == FGameplayTag::RequestGameplayTag("Status.Blizzard"))
	{
		if (bAdded)
		{
			AddBlizzard();
		}
		else
		{
			RemoveBlizzard();
		}
}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Burn"))
	{
		if (bAdded)
		{
			AddBurn();
		}
		else
		{
			RemoveBurn();
		}
}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Burst"))
	{
		if (bAdded)
		{
			AddBurst();
		}
		else
		{
			RemoveBurst();
		}
}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Char"))
	{
		if (bAdded)
		{
			AddChar();
		}
		else
		{
			RemoveChar();
		}
}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Charge"))
	{
		if (bAdded)
		{
			AddCharge();
		}
		else
		{
			RemoveCharge();
		}
}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Conductor"))
	{
		if (bAdded)
		{
			AddConductor();
		}
		else
		{
			RemoveConductor();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Crystalize"))
	{
		if (bAdded)
		{
			AddCrystalize();
		}
		else
		{
			RemoveCrystalize();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Earthquake"))
	{
		if (bAdded)
		{
			AddEarthquake();
		}
		else
		{
			RemoveEarthquake();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Ember"))
	{
		if (bAdded)
		{
			AddEmber();
		}
		else
		{
			RemoveEmber();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Energize"))
	{
		if (bAdded)
		{
			AddEnergize();
		}
		else
		{
			RemoveEnergize();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Entomb"))
	{
		if (bAdded)
		{
			AddEntomb();
		}
		else
		{
			RemoveEntomb();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Explode"))
	{
		if (bAdded)
		{
			AddExplode();
		}
		else
		{
			RemoveExplode();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Extinguish"))
	{
		if (bAdded)
		{
			AddExtinguish();
		}
		else
		{
			RemoveExtinguish();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Fracture"))
	{
		if (bAdded)
		{
			AddFracture();
		}
		else
		{
			RemoveFracture();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Freeze"))
	{
		if (bAdded)
		{
			AddFreeze();
		}
		else
		{
			RemoveFreeze();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Hypercharge"))
	{
		if (bAdded)
		{
			AddHypercharge();
		}
		else
		{
			RemoveHypercharge();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Lava"))
	{
		if (bAdded)
		{
			AddLava();
		}
		else
		{
			RemoveLava();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Magma"))
	{
		if (bAdded)
		{
			AddMagma();
		}
		else
		{
			RemoveMagma();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Melt"))
	{
		if (bAdded)
		{
			AddMelt();
		}
		else
		{
			RemoveMelt();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Overcharge"))
	{
		if (bAdded)
		{
			AddOvercharge();
		}
		else
		{
			RemoveOvercharge();
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
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Shockwave"))
	{
		if (bAdded)
		{
			AddShockwave();
		}
		else
		{
			RemoveShockwave();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Singe"))
	{
		if (bAdded)
		{
			AddSinge();
		}
		else
		{
			RemoveSinge();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Slow"))
	{
		if (bAdded)
		{
			AddSlow();
		}
		else
		{
			RemoveSlow();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Smolder"))
	{
		if (bAdded)
		{
			AddSmolder();
		}
		else
		{
			RemoveSmolder();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Stagger"))
	{
		if (bAdded)
		{
			AddStagger();
		}
		else
		{
			RemoveStagger();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Storm"))
	{
		if (bAdded)
		{
			AddStorm();
		}
		else
		{
			RemoveStorm();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Stun"))
	{
		if (bAdded)
		{
			AddStun();
		}
		else
		{
			RemoveStun();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Supercharge"))
	{
		if (bAdded)
		{
			AddSupercharge();
		}
		else
		{
			RemoveSupercharge();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Superheat"))
	{
		if (bAdded)
		{
			AddSuperheat();
		}
		else
		{
			RemoveSuperheat();
		}
		}
	else if (tag == FGameplayTag::RequestGameplayTag("Status.Thunderbolt"))
	{
		if (bAdded)
		{
			AddThunderbolt();
		}
		else
		{
			RemoveThunderbolt();
		}
		}
	
}









void UStatusComponent::AddBlizzard()
{
	// Add Blizzard logic here
}

void UStatusComponent::RemoveBlizzard()
{
	// Remove Blizzard logic here
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
	if (TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Burn")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Burn"));
	}
}

void UStatusComponent::BurnDamageTickFunction()
{
	UE_LOG(LogTemp, Warning, TEXT("Ow!"));
	UHealthComponent* HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
	if (HealthComponent)
	{
		HealthComponent->TakeFlatDamage(BurnDamageTick, nullptr);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No HealthComponent found on owner!"));
	}

	GetWorld()->GetTimerManager().SetTimer(BurnDamageTickHandle, this, &UStatusComponent::BurnDamageTickFunction, BurnDamageTickInterval, true);
}

void UStatusComponent::AddBurst()
{
	// Add Burst logic here
}

void UStatusComponent::RemoveBurst()
{
	// Remove Burst logic here
}

void UStatusComponent::AddChar()
{
	// Add Char logic here
}

void UStatusComponent::RemoveChar()
{
	// Remove Char logic here
}

void UStatusComponent::AddCharge()
{
	// Add Charge logic here
}

void UStatusComponent::RemoveCharge()
{
	// Remove Charge logic here
}

void UStatusComponent::AddConductor()
{
	// Add Conductor logic here
}

void UStatusComponent::RemoveConductor()
{
	// Remove Conductor logic here
}

void UStatusComponent::AddCrystalize()
{
	// Add Crystalize logic here
}

void UStatusComponent::RemoveCrystalize()
{
	// Remove Crystalize logic here
}

void UStatusComponent::AddEarthquake()
{
	// Add Earthquake logic here
}

void UStatusComponent::RemoveEarthquake()
{
	// Remove Earthquake logic here
}

void UStatusComponent::AddEmber()
{
	// Add Ember logic here
}

void UStatusComponent::RemoveEmber()
{
	// Remove Ember logic here
}

void UStatusComponent::AddEnergize()
{
	// Add Energize logic here
}

void UStatusComponent::RemoveEnergize()
{
	// Remove Energize logic here
}

void UStatusComponent::AddEntomb()
{
	// Add Entomb logic here
}

void UStatusComponent::RemoveEntomb()
{
	// Remove Entomb logic here
}

void UStatusComponent::AddExplode()
{
	// Add Explode logic here
}

void UStatusComponent::RemoveExplode()
{
	// Remove Explode logic here
}

void UStatusComponent::AddExtinguish()
{
	// Add Extinguish logic here
}

void UStatusComponent::RemoveExtinguish()
{
	// Remove Extinguish logic here
}

void UStatusComponent::AddFracture()
{
	// Add Fracture logic here
}

void UStatusComponent::RemoveFracture()
{
	// Remove Fracture logic here
}

void UStatusComponent::AddFreeze()
{
	// Add Freeze logic here
}

void UStatusComponent::RemoveFreeze()
{
	// Remove Freeze logic here
}

void UStatusComponent::AddHypercharge()
{
	// Add Hypercharge logic here
}

void UStatusComponent::RemoveHypercharge()
{
	// Remove Hypercharge logic here
}

void UStatusComponent::AddLava()
{
	// Add Lava logic here
}

void UStatusComponent::RemoveLava()
{
	// Remove Lava logic here
}

void UStatusComponent::AddMagma()
{
	// Add Magma logic here
}

void UStatusComponent::RemoveMagma()
{
	// Remove Magma logic here
}

void UStatusComponent::AddMelt()
{
	// Add Melt logic here
}

void UStatusComponent::RemoveMelt()
{
	// Remove Melt logic here
}

void UStatusComponent::AddOvercharge()
{
	// Add Overcharge logic here
}

void UStatusComponent::RemoveOvercharge()
{
	// Remove Overcharge logic here
}

void UStatusComponent::AddShield()
{
	UE_LOG(LogTemp, Warning, TEXT("Shielded!"));
	UHealthComponent* HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
	if (HealthComponent)
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
	if (HealthComponent)
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
	if (TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Shield")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Shield"));
	}
}

void UStatusComponent::AddShockwave()
{
	// Add Shockwave logic here
}

void UStatusComponent::RemoveShockwave()
{
	// Remove Shockwave logic here
}

void UStatusComponent::AddSinge()
{
	// Add Singe logic here
}

void UStatusComponent::RemoveSinge()
{
	// Remove Singe logic here
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
	if (TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Slow")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Slow"));
	}
}

void UStatusComponent::AddSmolder()
{
	// Add Smolder logic here
}

void UStatusComponent::RemoveSmolder()
{
	// Remove Smolder logic here
}

void UStatusComponent::AddStagger()
{
	// Add Stagger logic here
}

void UStatusComponent::RemoveStagger()
{
	// Remove Stagger logic here
}

void UStatusComponent::AddStorm()
{
	// Add Storm logic here
}

void UStatusComponent::RemoveStorm()
{
	// Remove Storm logic here
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
	if (TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Stun")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Stun"));
	}
}

void UStatusComponent::AddSupercharge()
{
	// Add Supercharge logic here
}

void UStatusComponent::RemoveSupercharge()
{
	// Remove Supercharge logic here
}

void UStatusComponent::AddSuperheat()
{
	// Add Superheat logic here
}

void UStatusComponent::RemoveSuperheat()
{
	// Remove Superheat logic here
}

void UStatusComponent::AddThunderbolt()
{
	// Add Thunderbolt logic here
}

void UStatusComponent::RemoveThunderbolt()
{
	// Remove Thunderbolt logic here
}


