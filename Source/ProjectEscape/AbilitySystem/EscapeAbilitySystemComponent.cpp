// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/EscapeAbilitySystemComponent.h"
//#include "AbilitySystem/Abilities/EscapeGameplayAbility.h"
#include "AbilitySystem/Abilities/EscapeSurvivorGameplayAbility.h"
#include "Net/UnrealNetwork.h"

#include "EscapeDebugHelper.h"


UEscapeAbilitySystemComponent::UEscapeAbilitySystemComponent()
{
	SetIsReplicatedByDefault(true);

}

void UEscapeAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag)) continue;

		if (AbilitySpec.IsActive())
		{
			/*Debug::Print(FString::Printf(TEXT("Ability %s is already active, skipping activation on %s"),
				*AbilitySpec.GetDynamicSpecSourceTags().ToString(),
				GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client")));*/

			continue;
		}

		TryActivateAbility(AbilitySpec.Handle);

		//Debug::Print(FString::Printf(TEXT("Ability %s has been triggered %s"), *AbilitySpec.GetDynamicSpecSourceTags().ToString(), GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client")));
	}
}

void UEscapeAbilitySystemComponent::OnAbilityInputRelease(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
        if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag))
		{
			continue;
		}

		if (AbilitySpec.IsActive())
		{
			if (const UEscapeSurvivorGameplayAbility* SurvivorGameplayAbility = Cast<UEscapeSurvivorGameplayAbility>(AbilitySpec.Ability))
			{
				if (SurvivorGameplayAbility->bShouldCancelOnRelease)
				{
					CancelAbilityHandle(AbilitySpec.Handle);
				}
			}
		}
	}
}

void UEscapeAbilitySystemComponent::GrantSurvivorWeaponAbility(const TArray<FEscapeSurvivorAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	if (InDefaultWeaponAbilities.IsEmpty())
	{
		return;
	}
	
	for (const FEscapeSurvivorAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid()) continue;


		if (HasAbilityOfTag(AbilitySet.InputTag))
		{
			//Debug::Print(FString::Printf(TEXT("Skipping ability %s, already granted"), *AbilitySet.InputTag.ToString()));
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));

		//Debug::Print(FString::Printf(TEXT("AbilityToGrant %s has been triggered %s"), *AbilitySpec.GetDynamicSpecSourceTags().ToString(), GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client")));
	}
}

void UEscapeAbilitySystemComponent::RemoveGrantedSurvivorWeaponAbility(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	if (InSpecHandlesToRemove.IsEmpty())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
	{
		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}

	InSpecHandlesToRemove.Empty();
}

bool UEscapeAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	check(AbilityTagToActivate.IsValid());
	
	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;

	 GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);

	 if (!FoundAbilitySpecs.IsEmpty())
	 {
		 const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		 FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

		 check(SpecToActivate);

		 if (!SpecToActivate->IsActive())
		 {
			 return TryActivateAbility(SpecToActivate->Handle);
		 }
	 }
	 return false; 
}

void UEscapeAbilitySystemComponent::Server_AddGameplayTagIfNone_Implementation(FGameplayTag TagToAdd)
{
	if (GetOwner()->HasAuthority())
	{
		TagsToAdd = TagToAdd;
		TagsToRemove = FGameplayTag();
		AddLooseGameplayTag(TagToAdd);
	}
}

void UEscapeAbilitySystemComponent::Server_RemoveGameplayTagIfFound_Implementation(FGameplayTag TagToRemove)
{
	if (GetOwner()->HasAuthority() && HasMatchingGameplayTag(TagToRemove))
	{
		TagsToRemove = TagToRemove;
		TagsToAdd = FGameplayTag();
		RemoveLooseGameplayTag(TagToRemove);
	}
}

void UEscapeAbilitySystemComponent::OnRep_AddGameplayTags()
{
	AddLooseGameplayTag(TagsToAdd);
}

void UEscapeAbilitySystemComponent::OnRep_RemoveGameplayTags()
{
	RemoveLooseGameplayTag(TagsToRemove);
}

void UEscapeAbilitySystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEscapeAbilitySystemComponent, TagsToAdd);
	DOREPLIFETIME(UEscapeAbilitySystemComponent, TagsToRemove);
}

bool UEscapeAbilitySystemComponent::HasAbilityOfTag(const FGameplayTag& InTag)
{
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items) 
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InTag))
		{
			return true; 
		}
	}
	return false;
}
