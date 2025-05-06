// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/EscapeAbilitySystemComponent.h"
#include "EscapeAttributeSet.generated.h"

class IPawnUIInterface;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UEscapeAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UEscapeAttributeSet();

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

#pragma region Character

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_CurrentHealth, Category = "Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UEscapeAttributeSet, CurrentHealth);

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UEscapeAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UEscapeAttributeSet, AttackPower);

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Damage")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UEscapeAttributeSet, DefensePower);

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Damage")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UEscapeAttributeSet, DamageTaken);

#pragma endregion


#pragma region Weapon

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Weapon")
	FGameplayAttributeData MaxWeaponAmmunitionAmount;
	ATTRIBUTE_ACCESSORS(UEscapeAttributeSet, MaxWeaponAmmunitionAmount);

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_CurrentWeaponAmmunitionAmount, Category = "Weapon")
	FGameplayAttributeData CurrentWeaponAmmunitionAmount;
	ATTRIBUTE_ACCESSORS(UEscapeAttributeSet, CurrentWeaponAmmunitionAmount);

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Weapon")
	FGameplayAttributeData TotalAmmunitionConsumed;
	ATTRIBUTE_ACCESSORS(UEscapeAttributeSet, TotalAmmunitionConsumed);

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Weapon")
	FGameplayAttributeData AmmunitionToReload;
	ATTRIBUTE_ACCESSORS(UEscapeAttributeSet, AmmunitionToReload);

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_SpareAmmunitionAmount, Category = "Weapon")
	FGameplayAttributeData SpareAmmunitionAmount;
	ATTRIBUTE_ACCESSORS(UEscapeAttributeSet, SpareAmmunitionAmount);

#pragma endregion
	

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_CurrentHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_CurrentWeaponAmmunitionAmount(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_SpareAmmunitionAmount(const FGameplayAttributeData& OldValue);

private:

	TWeakInterfacePtr<IPawnUIInterface> CachedPawnUIInterface;
};
