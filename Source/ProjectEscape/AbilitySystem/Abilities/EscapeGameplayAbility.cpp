// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/EscapeGameplayAbility.h"
#include "AbilitySystem/EscapeAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EscapeDebugHelper.h"


void UEscapeGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	if (AbilityActivationPolicy == EEscapeAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UEscapeGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == EEscapeAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			if (GetAvatarActorFromActorInfo()->HasAuthority())
			{
				ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
				//Debug::Print(FString::Printf(TEXT("Network role: %s is calling EndAbility"), ActorInfo->AvatarActor.Get()->HasAuthority() ? TEXT("Server") : TEXT("Client")));
			}
		}
	}
}

UPawnCombatComponent* UEscapeGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UEscapeAbilitySystemComponent* UEscapeGameplayAbility::GetEscapeAbilitySystemComponentFromActorInfo() const
{
	return Cast<UEscapeAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UEscapeGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	check(TargetASC && InSpecHandle.IsValid());

	return GetEscapeAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);
}

FActiveGameplayEffectHandle UEscapeGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EEscapeSuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EEscapeSuccessType::Successful : EEscapeSuccessType::Failed;

	return ActiveGameplayEffectHandle;
}
