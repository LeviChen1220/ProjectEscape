// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EscapeTypes/EscapeEnumTypes.h"
#include "EscapeGameplayAbility.generated.h"


class UPawnCombatComponent;
class UEscapeAbilitySystemComponent;

UENUM(BlueprintType)
enum class EEscapeAbilityActivationPolicy : uint8
{
	OnTriggerd,
	OnGiven
};

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UEscapeGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	EEscapeAbilityActivationPolicy AbilityActivationPolicy = EEscapeAbilityActivationPolicy::OnTriggerd;
	
	UFUNCTION(BlueprintPure, Category = "Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	UEscapeAbilitySystemComponent* GetEscapeAbilitySystemComponentFromActorInfo() const;

	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "Ability", meta = (DisplayName = "Apply GameplayEffectSpecHandle To Target Actor", ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EEscapeSuccessType& OutSuccessType);
	
};
