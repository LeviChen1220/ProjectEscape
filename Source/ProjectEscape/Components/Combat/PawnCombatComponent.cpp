// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PawnCombatComponent.h"
#include "Items/Weapons/EscapeWeaponBase.h"
#include "Components/BoxComponent.h"
#include "EscapeGameplayTags.h"

#include "EscapeDebugHelper.h"
#include "Net/UnrealNetwork.h"

void UPawnCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPawnCombatComponent, CurrentEquippedWeaponTag);
	DOREPLIFETIME(UPawnCombatComponent, CharacterCarriedWeapons);

}

void UPawnCombatComponent::OnRep_CarriedWeapons()
{
	CharacterCarriedWeaponMap.Empty();

	for (const FCarriedWeaponInfo& WeaponInfo : CharacterCarriedWeapons)
	{
		if (WeaponInfo.Weapon.IsValid())
		{
			CharacterCarriedWeaponMap.Emplace(WeaponInfo.WeaponTag, WeaponInfo.Weapon.Get());
		}
	}
}

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AEscapeWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister), TEXT("%s has already been added as carried weapon"),*InWeaponTagToRegister.ToString());
	check(InWeaponToRegister);

	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTagToRegister;
	}

	FCarriedWeaponInfo NewWeaponInfo;
	NewWeaponInfo.WeaponTag = InWeaponTagToRegister;
	NewWeaponInfo.Weapon = InWeaponToRegister;
	CharacterCarriedWeapons.Add(NewWeaponInfo);

	if (GetOwner())
	{
		GetOwner()->ForceNetUpdate();
	}

	const FString WeaponString = FString::Printf(TEXT("%s weapon has been registed using the tag %s %s"), *InWeaponToRegister->GetName(), *InWeaponTagToRegister.ToString(), GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client"));
	//Debug::Print(WeaponString);
}

AEscapeWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
	{
		if (AEscapeWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}
	return nullptr;
}

void UPawnCombatComponent::OnRep_CurrentEquippedWeaponTag()
{

}

AEscapeWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}

	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		AEscapeWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon();

		check(WeaponToToggle);

		if (bShouldEnable)
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			OverlappedActors.Empty();
		}
	}

	if (ToggleDamageType == EToggleDamageType::LeftHand)
	{
		AEscapeWeaponBase* WeaponToToggle = GetCharacterCarriedWeaponByTag(EscapeGameplayTags::Enemy_Weapon_Left);

		check(WeaponToToggle);

		if (bShouldEnable)
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			OverlappedActors.Empty();
		}
	}

	if (ToggleDamageType == EToggleDamageType::RightHand)
	{
		AEscapeWeaponBase* WeaponToToggle = GetCharacterCarriedWeaponByTag(EscapeGameplayTags::Enemy_Weapon_Right);

		check(WeaponToToggle);

		if (bShouldEnable)
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			OverlappedActors.Empty();
		}
	}
}

void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
{
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
}



