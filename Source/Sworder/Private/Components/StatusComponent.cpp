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

void UStatusComponent::HandleStatusTag(FGameplayTag tag, bool bAdded, AActor* ComboCauser)
{

	if (tag == FGameplayTag::RequestGameplayTag("Status.Blizzard"))
	{
		if (bAdded)
		{
			AddBlizzard(ComboCauser);
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
			AddConductor(ComboCauser);
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
			AddEarthquake(ComboCauser);
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
			AddEmber(ComboCauser);
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
			AddExplode(ComboCauser);
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
			AddHypercharge(ComboCauser);
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
			AddLava(ComboCauser);
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
			AddShockwave(ComboCauser);
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
			AddSmolder(ComboCauser);
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
			AddStorm(ComboCauser);
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
			AddSupercharge(ComboCauser);
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









void UStatusComponent::AddBlizzard(AActor* ComboCauser)
{
	// Add Blizzard logic here

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}


	const FGameplayTag BlizzardTag = FGameplayTag::RequestGameplayTag("Status.Blizzard");
	UE_LOG(LogTemp, Warning, TEXT("ComboCauser: %s"), *ComboCauser->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OwnerActor: %s"), *OwnerActor->GetName());
	OnStatusEffectAreaRequested.Broadcast(BlizzardTag, true, OwnerActor, ComboCauser, OwnerActor->GetActorLocation());
	GetWorld()->GetTimerManager().SetTimer(BlizzardTimerHandle, this, &UStatusComponent::RemoveBlizzard, BlizzardTimer, false);
}

void UStatusComponent::RemoveBlizzard()
{
	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}

	const FGameplayTag BlizzardTag = FGameplayTag::RequestGameplayTag("Status.Blizzard");

	OnStatusEffectAreaRequested.Broadcast(BlizzardTag, false, OwnerActor, nullptr, OwnerActor->GetActorLocation());

	UTagManager* TagManager = GetOwner()->FindComponentByClass<UTagManager>();
	if (TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Blizzard")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Blizzard"));
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
	AddStatusTag.Broadcast(FGameplayTag::RequestGameplayTag("Status.Char"), true);
	UE_LOG(LogTemp, Warning, TEXT("Charred!"));
	GetWorld()->GetTimerManager().SetTimer(CharTimerHandle, this, &UStatusComponent::RemoveChar, CharTimer, false);
}

void UStatusComponent::RemoveChar()
{
	AddStatusTag.Broadcast(FGameplayTag::RequestGameplayTag("Status.Char"), false);
	UE_LOG(LogTemp, Warning, TEXT("Char removed!"));
	GetWorld()->GetTimerManager().ClearTimer(CharTimerHandle);

	UTagManager* TagManager = GetOwner()->FindComponentByClass<UTagManager>();
	if (TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Char")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Char"));
	}
}

void UStatusComponent::AddCharge()
{
	// Add Charge logic here
}

void UStatusComponent::RemoveCharge()
{
	// Remove Charge logic here
}

void UStatusComponent::AddConductor(AActor* ComboCauser)
{

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}


	const FGameplayTag ConductorTag = FGameplayTag::RequestGameplayTag("Status.Conductor");
	UE_LOG(LogTemp, Warning, TEXT("ComboCauser: %s"), *ComboCauser->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OwnerActor: %s"), *OwnerActor->GetName());
	OnStatusEffectAreaRequested.Broadcast(ConductorTag, true, OwnerActor, ComboCauser, OwnerActor->GetActorLocation());
	GetWorld()->GetTimerManager().SetTimer(ConductorTimerHandle, this, &UStatusComponent::RemoveConductor, ConductorTimer, false);
}

void UStatusComponent::RemoveConductor()
{
	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}

	const FGameplayTag ConductorTag = FGameplayTag::RequestGameplayTag("Status.Conductor");

	OnStatusEffectAreaRequested.Broadcast(ConductorTag, false, OwnerActor, nullptr, OwnerActor->GetActorLocation());

	UTagManager* TagManager = GetOwner()->FindComponentByClass<UTagManager>();
	if (TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Conductor")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Conductor"));
	}

}

void UStatusComponent::AddCrystalize()
{
	// Add Crystalize logic here
}

void UStatusComponent::RemoveCrystalize()
{
	// Remove Crystalize logic here
}

void UStatusComponent::AddEarthquake(AActor* ComboCauser)
{
	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}

	const FGameplayTag EarthquakeTag = FGameplayTag::RequestGameplayTag("Status.Earthquake");
	UE_LOG(LogTemp, Warning, TEXT("ComboCauser: %s"), *ComboCauser->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OwnerActor: %s"), *OwnerActor->GetName());
	OnStatusEffectAreaRequested.Broadcast(EarthquakeTag, true, OwnerActor, ComboCauser, OwnerActor->GetActorLocation());
	GetWorld()->GetTimerManager().SetTimer(EarthquakeTimerHandle, this, &UStatusComponent::RemoveEarthquake, EarthquakeTimer, false);
}

void UStatusComponent::RemoveEarthquake()
{
	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}

	const FGameplayTag EarthquakeTag = FGameplayTag::RequestGameplayTag("Status.Earthquake");

	OnStatusEffectAreaRequested.Broadcast(EarthquakeTag, false, OwnerActor, nullptr, OwnerActor->GetActorLocation());

	UTagManager* TagManager = GetOwner()->FindComponentByClass<UTagManager>();
	if (TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Earthquake")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Earthquake"));
	}

}

void UStatusComponent::AddEmber(AActor* ComboCauser)
{
	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}
	

	const FGameplayTag EmberTag = FGameplayTag::RequestGameplayTag("Status.Ember");
	UE_LOG(LogTemp, Warning, TEXT("ComboCauser: %s"), *ComboCauser->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OwnerActor: %s"), *OwnerActor->GetName());
	OnStatusEffectAreaRequested.Broadcast(EmberTag, true, OwnerActor, ComboCauser, OwnerActor->GetActorLocation());
	GetWorld()->GetTimerManager().SetTimer(EmberTimerHandle, this, &UStatusComponent::RemoveEmber, EmberTimer, false);
}

void UStatusComponent::RemoveEmber()
{
	// Remove Ember logic here
	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}

	const FGameplayTag EmberTag = FGameplayTag::RequestGameplayTag("Status.Ember");

	OnStatusEffectAreaRequested.Broadcast(EmberTag, false, OwnerActor, nullptr, OwnerActor->GetActorLocation());

	UTagManager* TagManager = GetOwner()->FindComponentByClass<UTagManager>();
	if (TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Ember")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Ember"));
	}

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

void UStatusComponent::AddExplode(AActor* ComboCauser)
{

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}


	const FGameplayTag ExplodeTag = FGameplayTag::RequestGameplayTag("Status.Explode");
	UE_LOG(LogTemp, Warning, TEXT("ComboCauser: %s"), *ComboCauser->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OwnerActor: %s"), *OwnerActor->GetName());
	OnStatusEffectAreaRequested.Broadcast(ExplodeTag, true, OwnerActor, ComboCauser, OwnerActor->GetActorLocation());
	GetWorld()->GetTimerManager().SetTimer(ExplodeTimerHandle, this, &UStatusComponent::RemoveExplode, ExplodeTimer, false);
}

void UStatusComponent::RemoveExplode()
{

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}

	const FGameplayTag ExplodeTag = FGameplayTag::RequestGameplayTag("Status.Explode");

	OnStatusEffectAreaRequested.Broadcast(ExplodeTag, false, OwnerActor, nullptr, OwnerActor->GetActorLocation());

	UTagManager* TagManager = GetOwner()->FindComponentByClass<UTagManager>();
	if (TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Explode")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Explode"));
	}
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

void UStatusComponent::AddHypercharge(AActor* ComboCauser)
{

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}


	const FGameplayTag HyperchargeTag = FGameplayTag::RequestGameplayTag("Status.Hypercharge");
	UE_LOG(LogTemp, Warning, TEXT("ComboCauser: %s"), *ComboCauser->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OwnerActor: %s"), *OwnerActor->GetName());
	OnStatusEffectAreaRequested.Broadcast(HyperchargeTag, true, OwnerActor, ComboCauser, OwnerActor->GetActorLocation());
	GetWorld()->GetTimerManager().SetTimer(HyperchargeTimerHandle, this, &UStatusComponent::RemoveHypercharge, HyperchargeTimer, false);
}

void UStatusComponent::RemoveHypercharge()
{

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}

	const FGameplayTag HyperchargeTag = FGameplayTag::RequestGameplayTag("Status.Hypercharge");

	OnStatusEffectAreaRequested.Broadcast(HyperchargeTag, false, OwnerActor, nullptr, OwnerActor->GetActorLocation());

	UTagManager* TagManager = GetOwner()->FindComponentByClass<UTagManager>();
	if (TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Hypercharge")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Hypercharge"));
	}
}

void UStatusComponent::AddLava(AActor* ComboCauser)
{

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}


	const FGameplayTag LavaTag = FGameplayTag::RequestGameplayTag("Status.Lava");
	UE_LOG(LogTemp, Warning, TEXT("ComboCauser: %s"), *ComboCauser->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OwnerActor: %s"), *OwnerActor->GetName());
	OnStatusEffectAreaRequested.Broadcast(LavaTag, true, OwnerActor, ComboCauser, OwnerActor->GetActorLocation());
	GetWorld()->GetTimerManager().SetTimer(LavaTimerHandle, this, &UStatusComponent::RemoveLava, LavaTimer, false);
}

void UStatusComponent::RemoveLava()
{

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}

	const FGameplayTag LavaTag = FGameplayTag::RequestGameplayTag("Status.Lava");

	OnStatusEffectAreaRequested.Broadcast(LavaTag, false, OwnerActor, nullptr, OwnerActor->GetActorLocation());

	UTagManager* TagManager = GetOwner()->FindComponentByClass<UTagManager>();
	if (TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Lava")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Lava"));
	}
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

void UStatusComponent::AddShockwave(AActor* ComboCauser)
{

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}


	const FGameplayTag ShockwaveTag = FGameplayTag::RequestGameplayTag("Status.Shockwave");
	UE_LOG(LogTemp, Warning, TEXT("ComboCauser: %s"), *ComboCauser->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OwnerActor: %s"), *OwnerActor->GetName());
	OnStatusEffectAreaRequested.Broadcast(ShockwaveTag, true, OwnerActor, ComboCauser, OwnerActor->GetActorLocation());
	GetWorld()->GetTimerManager().SetTimer(ShockwaveTimerHandle, this, &UStatusComponent::RemoveShockwave, ShockwaveTimer, false);
}

void UStatusComponent::RemoveShockwave()
{

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}

	const FGameplayTag ShockwaveTag = FGameplayTag::RequestGameplayTag("Status.Shockwave");

	OnStatusEffectAreaRequested.Broadcast(ShockwaveTag, false, OwnerActor, nullptr, OwnerActor->GetActorLocation());

	UTagManager* TagManager = GetOwner()->FindComponentByClass<UTagManager>();
	if (TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Shockwave")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Shockwave"));
	}
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

void UStatusComponent::AddSmolder(AActor* ComboCauser)
{

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}


	const FGameplayTag SmolderTag = FGameplayTag::RequestGameplayTag("Status.Smolder");
	UE_LOG(LogTemp, Warning, TEXT("ComboCauser: %s"), *ComboCauser->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OwnerActor: %s"), *OwnerActor->GetName());
	OnStatusEffectAreaRequested.Broadcast(SmolderTag, true, OwnerActor, ComboCauser, OwnerActor->GetActorLocation());
	GetWorld()->GetTimerManager().SetTimer(SmolderTimerHandle, this, &UStatusComponent::RemoveSmolder, SmolderTimer, false);
}

void UStatusComponent::RemoveSmolder()
{

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}

	const FGameplayTag SmolderTag = FGameplayTag::RequestGameplayTag("Status.Smolder");

	OnStatusEffectAreaRequested.Broadcast(SmolderTag, false, OwnerActor, nullptr, OwnerActor->GetActorLocation());

	UTagManager* TagManager = GetOwner()->FindComponentByClass<UTagManager>();
	if (TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Smolder")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Smolder"));
	}
}

void UStatusComponent::AddStagger()
{
	// Add Stagger logic here
}

void UStatusComponent::RemoveStagger()
{
	// Remove Stagger logic here
}

void UStatusComponent::AddStorm(AActor* ComboCauser)
{

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}


	const FGameplayTag StormTag = FGameplayTag::RequestGameplayTag("Status.Storm");
	UE_LOG(LogTemp, Warning, TEXT("ComboCauser: %s"), *ComboCauser->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OwnerActor: %s"), *OwnerActor->GetName());
	OnStatusEffectAreaRequested.Broadcast(StormTag, true, OwnerActor, ComboCauser, OwnerActor->GetActorLocation());
	GetWorld()->GetTimerManager().SetTimer(StormTimerHandle, this, &UStatusComponent::RemoveStorm, StormTimer, false);
}

void UStatusComponent::RemoveStorm()
{

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}

	const FGameplayTag StormTag = FGameplayTag::RequestGameplayTag("Status.Storm");

	OnStatusEffectAreaRequested.Broadcast(StormTag, false, OwnerActor, nullptr, OwnerActor->GetActorLocation());

	UTagManager* TagManager = GetOwner()->FindComponentByClass<UTagManager>();
	if (TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Storm")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Storm"));
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
	if (TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Stun")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Stun"));
	}
}

void UStatusComponent::AddSupercharge(AActor* ComboCauser)
{

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}


	const FGameplayTag SuperchargeTag = FGameplayTag::RequestGameplayTag("Status.Supercharge");
	UE_LOG(LogTemp, Warning, TEXT("ComboCauser: %s"), *ComboCauser->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OwnerActor: %s"), *OwnerActor->GetName());
	OnStatusEffectAreaRequested.Broadcast(SuperchargeTag, true, OwnerActor, ComboCauser, OwnerActor->GetActorLocation());
	GetWorld()->GetTimerManager().SetTimer(SuperchargeTimerHandle, this, &UStatusComponent::RemoveSupercharge, SuperchargeTimer, false);
}

void UStatusComponent::RemoveSupercharge()
{

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}

	const FGameplayTag SuperchargeTag = FGameplayTag::RequestGameplayTag("Status.Supercharge");

	OnStatusEffectAreaRequested.Broadcast(SuperchargeTag, false, OwnerActor, nullptr, OwnerActor->GetActorLocation());

	UTagManager* TagManager = GetOwner()->FindComponentByClass<UTagManager>();
	if (TagManager && TagManager->GetGameplayTagContainer().HasTag(FGameplayTag::RequestGameplayTag("Status.Supercharge")))
	{
		TagManager->GameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Supercharge"));
	}
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


