// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ComboDataAsset.h"

void UComboDataAsset::GenerateCombos()
{
    StoredCombos.Empty();

    // ---------------------------------------------------------
    // ELEMENT TAGS
    // ---------------------------------------------------------

    const FGameplayTag Fire =
        FGameplayTag::RequestGameplayTag(FName("Element.Fire"));

    const FGameplayTag Ice =
        FGameplayTag::RequestGameplayTag(FName("Element.Ice"));

    const FGameplayTag Lightning =
        FGameplayTag::RequestGameplayTag(FName("Element.Lightning"));

    const FGameplayTag Earth =
        FGameplayTag::RequestGameplayTag(FName("Element.Earth"));

    const FGameplayTag Neutral =
        FGameplayTag::RequestGameplayTag(FName("Element.Neutral"));



    // ---------------------------------------------------------
    // RESULT TAGS
    // ---------------------------------------------------------

    const FGameplayTag Burn =
        FGameplayTag::RequestGameplayTag(FName("Status.Burn"));

    const FGameplayTag Ember =
        FGameplayTag::RequestGameplayTag(FName("Status.Ember"));

    const FGameplayTag Char =
        FGameplayTag::RequestGameplayTag(FName("Status.Char"));

    const FGameplayTag Magma =
        FGameplayTag::RequestGameplayTag(FName("Status.Magma"));

    const FGameplayTag Explode =
        FGameplayTag::RequestGameplayTag(FName("Status.Explode"));

    const FGameplayTag Superheat =
        FGameplayTag::RequestGameplayTag(FName("Status.Superheat"));

    const FGameplayTag Extinguish =
        FGameplayTag::RequestGameplayTag(FName("Status.Extinguish"));

    const FGameplayTag Slow =
        FGameplayTag::RequestGameplayTag(FName("Status.Slow"));

    const FGameplayTag Freeze =
        FGameplayTag::RequestGameplayTag(FName("Status.Freeze"));

    const FGameplayTag Blizzard =
        FGameplayTag::RequestGameplayTag(FName("Status.Blizzard"));

    const FGameplayTag Melt =
        FGameplayTag::RequestGameplayTag(FName("Status.Melt"));

    const FGameplayTag Fracture =
        FGameplayTag::RequestGameplayTag(FName("Status.Fracture"));

    const FGameplayTag Conductor =
        FGameplayTag::RequestGameplayTag(FName("Status.Conductor"));

    const FGameplayTag Charge =
        FGameplayTag::RequestGameplayTag(FName("Status.Charge"));

    const FGameplayTag Energize =
        FGameplayTag::RequestGameplayTag(FName("Status.Energize"));

    const FGameplayTag Thunderbolt =
        FGameplayTag::RequestGameplayTag(FName("Status.Thunderbolt"));

    const FGameplayTag Stun =
        FGameplayTag::RequestGameplayTag(FName("Status.Stun"));

    const FGameplayTag Singe =
        FGameplayTag::RequestGameplayTag(FName("Status.Singe"));

    const FGameplayTag Overcharge =
        FGameplayTag::RequestGameplayTag(FName("Status.Overcharge"));

    const FGameplayTag Shield =
        FGameplayTag::RequestGameplayTag(FName("Status.Shield"));

    const FGameplayTag Entomb =
        FGameplayTag::RequestGameplayTag(FName("Status.Entomb"));

    const FGameplayTag Earthquake =
        FGameplayTag::RequestGameplayTag(FName("Status.Earthquake"));

    const FGameplayTag Lava =
        FGameplayTag::RequestGameplayTag(FName("Status.Lava"));

    const FGameplayTag Smolder =
        FGameplayTag::RequestGameplayTag(FName("Status.Smolder"));

    const FGameplayTag Shockwave =
        FGameplayTag::RequestGameplayTag(FName("Status.Shockwave"));

    const FGameplayTag Crystalize =
        FGameplayTag::RequestGameplayTag(FName("Status.Crystalize"));

    const FGameplayTag Supercharge =
        FGameplayTag::RequestGameplayTag(FName("Status.Supercharge"));

    const FGameplayTag Storm =
        FGameplayTag::RequestGameplayTag(FName("Status.Storm"));

    const FGameplayTag Hypercharge =
        FGameplayTag::RequestGameplayTag(FName("Status.Hypercharge"));

    const FGameplayTag Burst =
        FGameplayTag::RequestGameplayTag(FName("Status.Burst"));

    // FIRE

    AddCombo({ Fire }, Burn, false, false);

    AddCombo({ Fire, Fire }, Ember, false, false);

    AddCombo({ Fire, Fire, Fire }, Char, false, true);

    AddCombo({ Fire, Earth }, Char, false, true);

    AddCombo({ Fire, Fire, Earth }, Magma, false, false);

    AddCombo({ Fire, Lightning }, Explode, false, true);

    AddCombo({ Fire, Fire, Lightning }, Explode, false, true);

    AddCombo({ Fire, Fire, Earth, Lightning }, Superheat, false, true);

    AddCombo({ Fire, Ice }, Extinguish, false, true);

    AddCombo({ Fire, Fire, Ice }, Extinguish, false, true);

    AddCombo({ Fire, Fire, Earth, Ice }, Extinguish, false, true);


    // ICE

    AddCombo({ Ice }, Slow, false, false);

    AddCombo({ Ice, Ice }, Freeze, false, false);

    AddCombo({ Ice, Ice, Ice }, Blizzard, false, true);

    AddCombo({ Ice, Fire }, Melt, false, true);

    AddCombo({ Ice, Ice, Fire }, Melt, false, true);

    AddCombo({ Ice, Earth }, Fracture, false, true);

    AddCombo({ Ice, Ice, Earth }, Fracture, false, true);

    AddCombo({ Ice, Lightning }, Conductor, true, true);

    AddCombo({ Ice, Ice, Lightning }, Conductor, true, true);


    // LIGHTNING

    AddCombo({ Lightning }, Charge, false, false);

    AddCombo({ Lightning, Lightning }, Energize, true, false);

    AddCombo({ Lightning, Lightning, Lightning }, Thunderbolt, false, true);

    AddCombo({ Lightning, Earth }, Stun, false, true);

    AddCombo({ Lightning, Lightning, Earth }, Stun, false, true);

    AddCombo({ Lightning, Ice }, Conductor, true, true);

    AddCombo({ Lightning, Lightning, Ice }, Conductor, true, true);

    AddCombo({ Lightning, Fire }, Singe, false, false);

    AddCombo({ Lightning, Fire, Fire }, Explode, false, true);

    AddCombo({ Lightning, Lightning, Fire }, Overcharge, false, true);


    // EARTH

    AddCombo({ Earth }, Shield, true, false);

    AddCombo({ Earth, Earth }, Entomb, false, false);

    AddCombo({ Earth, Earth, Earth }, Earthquake, true, true);

    AddCombo({ Earth, Fire }, Lava, false, false);

    AddCombo({ Earth, Ice, Fire }, Lava, false, false);

    AddCombo({ Earth, Fire, Earth }, Smolder, false, false);

    AddCombo({ Earth, Ice, Fire, Earth }, Smolder, false, false);

    AddCombo({ Earth, Lightning }, Shockwave, true, true);

    AddCombo({ Earth, Ice }, Crystalize, false, false);


    // SPECIAL

    AddCombo({ Earth, Ice, Lightning }, Supercharge, false, false);

    AddCombo({ Earth, Ice, Lightning}, Storm, true, true);

    AddCombo({ Earth, Ice, Lightning, Fire }, Hypercharge, true, false);

    AddCombo({ Earth, Ice, Lightning, Fire, Neutral }, Burst, false, true);

    //FINISHED

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Generated %d element combos."),
        StoredCombos.Num()
    );
}

void UComboDataAsset::AddCombo(
    const TArray<FGameplayTag>& Sequence,
    FGameplayTag Result,
    bool bSelfTarget,
    bool bEndsCombo
)
{
    FElementCombo NewCombo;

    NewCombo.ComboSequence = Sequence;
    NewCombo.Result = Result;
    NewCombo.SelfOrEnemy = bSelfTarget;
    NewCombo.bEndsCombo = bEndsCombo;

    StoredCombos.Add(NewCombo);
}
