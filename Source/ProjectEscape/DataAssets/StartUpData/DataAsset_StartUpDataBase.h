// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UEscapeGameplayAbility;
class UEscapeAbilitySystemComponent;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:

	virtual void GiveToAbilitySystemComponent(UEscapeAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UEscapeGameplayAbility>> ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UEscapeGameplayAbility>> ReactiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;

	void GrantAbilities( const TArray<TSubclassOf<UEscapeGameplayAbility>>& InAbilitiesToGive, UEscapeAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
