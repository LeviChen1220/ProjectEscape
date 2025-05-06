// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"

class AEscapeWeaponBase;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand
};
/**
 * 
 */
USTRUCT(BlueprintType)
struct FCarriedWeaponInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "WeaponTag"))
	FGameplayTag WeaponTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TWeakObjectPtr<AEscapeWeaponBase> Weapon;
};

UCLASS()
class PROJECTESCAPE_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AEscapeWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	AEscapeWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentEquippedWeaponTag, Category = "Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION()
	void OnRep_CurrentEquippedWeaponTag();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	AEscapeWeaponBase* GetCharacterCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	virtual void OnHitTargetActor(AActor* HitActor);
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	TArray<AActor*> OverlappedActors;

private:

	TMap<FGameplayTag, AEscapeWeaponBase*> CharacterCarriedWeaponMap;
	
	UPROPERTY(ReplicatedUsing = OnRep_CarriedWeapons)
	TArray<FCarriedWeaponInfo> CharacterCarriedWeapons;

	UFUNCTION()
	void OnRep_CarriedWeapons();
};
