// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DataAsset_SurvivorStartUpData.h"
#include "AbilitySystem/Abilities/EscapeSurvivorGameplayAbility.h"
#include "AbilitySystem/EscapeAbilitySystemComponent.h"



void UDataAsset_SurvivorStartUpData::GiveToAbilitySystemComponent(UEscapeAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
    Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);
    
    for (const FEscapeSurvivorAbilitySet& AbilitySet : SurvivorStartUpAbilitySets)
    {
        if (!AbilitySet.IsValid()) continue;

        FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
        AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
        AbilitySpec.Level = ApplyLevel;
        AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

        InASCToGive->GiveAbility(AbilitySpec);
    }
}
