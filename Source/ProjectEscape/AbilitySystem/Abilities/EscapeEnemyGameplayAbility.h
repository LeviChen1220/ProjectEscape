// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/EscapeGameplayAbility.h"
#include "EscapeEnemyGameplayAbility.generated.h"

class AEscapeEnemyCharacter;
class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UEscapeEnemyGameplayAbility : public UEscapeGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Ability")
	AEscapeEnemyCharacter* GetEnemyCharacterFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category = "Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat);


private:

	TWeakObjectPtr<AEscapeEnemyCharacter> CachedEscapeEnemyCharacter;
};
