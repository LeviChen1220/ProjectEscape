// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/EscapeEnemyGameplayAbility.h"
#include "Characters/EscapeEnemyCharacter.h"
#include "AbilitySystem/EscapeAbilitySystemComponent.h"
#include "EscapeGameplayTags.h"


AEscapeEnemyCharacter* UEscapeEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedEscapeEnemyCharacter.IsValid())
	{
		CachedEscapeEnemyCharacter = Cast<AEscapeEnemyCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedEscapeEnemyCharacter.IsValid()? CachedEscapeEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UEscapeEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}

FGameplayEffectSpecHandle UEscapeEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
{
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetEscapeAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetEscapeAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), ContextHandle);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(EscapeGameplayTags::Shared_SetByCaller_BaseDamage, InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel()));

	return EffectSpecHandle;
}
