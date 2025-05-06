// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "SurvivorCombatComponent.generated.h"

class AEscapeSurvivorWeapon;
class AEscapeSurvivorCharacter;
/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API USurvivorCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Combat")
	AEscapeSurvivorWeapon* GetSurvivorCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	AEscapeSurvivorWeapon* GetSurvivorCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetSurvivorCurrentEquippedWeaponDamageAtLevel(float InLevel) const;

	virtual void OnHitTargetActor(AActor* HitActor) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override;

};
