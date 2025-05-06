// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/EscapeGameplayAbility.h"
#include "EscapeSurvivorGameplayAbility.generated.h"

class AEscapeSurvivorCharacter;
class AEscapeSurvivorController;
class USurvivorCombatComponent;
/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UEscapeSurvivorGameplayAbility : public UEscapeGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Ability")
	AEscapeSurvivorCharacter* GetSurvivorCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Ability")
	AEscapeSurvivorController* GetSurvivorControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Ability")
	USurvivorCombatComponent* GetSurvivorCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Ability")
	FGameplayEffectSpecHandle MakeSurvivorDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount);

	UFUNCTION(BlueprintPure, Category = "Ability")
	FGameplayEffectSpecHandle MakeWeaponReloadEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponReloadAmount, FGameplayTag InWeaponReloadTag);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	bool bShouldCancelOnRelease = false;

private:

	TWeakObjectPtr<AEscapeSurvivorCharacter> CachedEscapeSurvivorCharacter;
	TWeakObjectPtr<AEscapeSurvivorController> CachedEscapeSurvivorController;
	
};
