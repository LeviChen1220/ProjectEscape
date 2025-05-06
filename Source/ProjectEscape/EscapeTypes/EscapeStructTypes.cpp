// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapeTypes/EscapeStructTypes.h"
#include "AbilitySystem/Abilities/EscapeSurvivorGameplayAbility.h"

bool FEscapeSurvivorAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
