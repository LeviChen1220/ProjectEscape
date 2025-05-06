// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/SurvivorCombatComponent.h"
#include "Items/Weapons/EscapeSurvivorWeapon.h"
#include "Characters/EscapeSurvivorCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EscapeGameplayTags.h"

AEscapeSurvivorWeapon* USurvivorCombatComponent::GetSurvivorCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{ 
    return Cast<AEscapeSurvivorWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

AEscapeSurvivorWeapon* USurvivorCombatComponent::GetSurvivorCurrentEquippedWeapon() const
{
    return Cast<AEscapeSurvivorWeapon>(GetCharacterCurrentEquippedWeapon());
}

float USurvivorCombatComponent::GetSurvivorCurrentEquippedWeaponDamageAtLevel(float InLevel) const
{
    return GetSurvivorCurrentEquippedWeapon()->SurvivorWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}

void USurvivorCombatComponent::OnHitTargetActor(AActor* HitActor)
{
    if (OverlappedActors.Contains(HitActor))
    {
        return;
    }

    OverlappedActors.AddUnique(HitActor);

    FGameplayEventData Data;
    Data.Instigator = GetOwningPawn();
    Data.Target = HitActor;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), EscapeGameplayTags::Shared_Event_RangedHit, Data);

}

void USurvivorCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{

}


