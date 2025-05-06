// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/EscapeWeaponBase.h"
#include "EscapeTypes/EscapeStructTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "EscapeSurvivorWeapon.generated.h"

class AEscapeSurvivorCharacter;
class UGameplayEffect;
class UEscapeAbilitySystemComponent;
class UCameraShakeBase;
/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API AEscapeSurvivorWeapon : public AEscapeWeaponBase
{
	GENERATED_BODY()

public:

	AEscapeSurvivorWeapon();

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FEscapeSurvivorWeaponData SurvivorWeaponData;

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;

	UFUNCTION()
	FVector GetMuzzleLocation() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void FireWeapon();

	void SetEquippedWeaponData(const FEscapeSurvivorWeaponData& NewData);

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Damage")
	TSubclassOf<UGameplayEffect> DamageEffect; // Assign this in Blueprint
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Camera")
	TSubclassOf<UCameraShakeBase> CameraShake;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|SFX", meta = (Categories = "GameplayCue"))
	FGameplayTag GameplayCueTagHitImpcat;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|SFX", meta = (Categories = "GameplayCue"))
	FGameplayTag GameplayCueTagFire;


protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable, Category = "Animation")
	void MulticastPlayFireWeaponMontage();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastDrawWeaponTrace(FVector Start, FVector End, bool isHit);

	void ApplyRangedDamage(AActor* TargetActor);

private:

	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;

	UPROPERTY()
	AEscapeSurvivorCharacter* CachedOwner;
	UPROPERTY()
	APlayerController* CachedOwnerPlayerController;
	UPROPERTY()
	UAnimInstance* CachedAnimInstance;
	UPROPERTY()
	UAnimMontage* CachedFireMontage;
	UPROPERTY()
	USoundCue* CachedFireSound = nullptr;

	UEscapeAbilitySystemComponent* CachedOwningPawnASC;
};
