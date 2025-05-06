// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/EscapeSurvivorGameplayAbility.h"
#include "AbilitySystem/EscapeAbilitySystemComponent.h"
#include "Characters/EscapeSurvivorCharacter.h"
#include "Characters/EscapeSurvivorController.h"
#include "EscapeGameplayTags.h"


AEscapeSurvivorCharacter* UEscapeSurvivorGameplayAbility::GetSurvivorCharacterFromActorInfo()
{
	if (!CachedEscapeSurvivorCharacter.IsValid())
	{
		CachedEscapeSurvivorCharacter = Cast<AEscapeSurvivorCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedEscapeSurvivorCharacter.IsValid()? CachedEscapeSurvivorCharacter.Get() : nullptr;
}

AEscapeSurvivorController* UEscapeSurvivorGameplayAbility::GetSurvivorControllerFromActorInfo()
{
	if (!CachedEscapeSurvivorController.IsValid())
	{
		CachedEscapeSurvivorController = Cast<AEscapeSurvivorController>(CurrentActorInfo->OwnerActor->GetInstigatorController());
	}
	return CachedEscapeSurvivorController.IsValid()? CachedEscapeSurvivorController.Get() : nullptr;
}

USurvivorCombatComponent* UEscapeSurvivorGameplayAbility::GetSurvivorCombatComponentFromActorInfo()
{
	return GetSurvivorCharacterFromActorInfo()->GetSurvivorCombatComponent();
}

FGameplayEffectSpecHandle UEscapeSurvivorGameplayAbility::MakeSurvivorDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount)
{
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetEscapeAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetEscapeAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), ContextHandle);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(EscapeGameplayTags::Shared_SetByCaller_BaseDamage, InWeaponBaseDamage);

	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InUsedComboCount);
	}

	return EffectSpecHandle;
}

FGameplayEffectSpecHandle UEscapeSurvivorGameplayAbility::MakeWeaponReloadEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponReloadAmount, FGameplayTag InWeaponReloadTag)
{
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetEscapeAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetEscapeAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), ContextHandle);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(InWeaponReloadTag, InWeaponReloadAmount);


	return EffectSpecHandle;
}
