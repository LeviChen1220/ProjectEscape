// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "EscapeTypes/EscapeStructTypes.h"

#include "EscapeAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UEscapeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

	UEscapeAbilitySystemComponent();
	
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputRelease(const FGameplayTag& InInputTag);

	UFUNCTION(BlueprintCallable, Category = "Ability", meta = (ApplyLevel = "1"))
	void GrantSurvivorWeaponAbility(const TArray<FEscapeSurvivorAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void RemoveGrantedSurvivorWeaponAbility(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_AddGameplayTags)
	FGameplayTag TagsToAdd;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_RemoveGameplayTags)
	FGameplayTag TagsToRemove;

	UFUNCTION()
	void OnRep_AddGameplayTags();

	UFUNCTION()
	void OnRep_RemoveGameplayTags();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_AddGameplayTagIfNone(FGameplayTag TagToAdd);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_RemoveGameplayTagIfFound(FGameplayTag TagToRemove);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UFUNCTION()
	bool HasAbilityOfTag(const FGameplayTag& InTag);
};
