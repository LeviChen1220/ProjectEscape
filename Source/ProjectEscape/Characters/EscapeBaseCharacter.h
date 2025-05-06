// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"

#include "EscapeBaseCharacter.generated.h"

class UEscapeAbilitySystemComponent;
class UEscapeAttributeSet;
class UDataAsset_StartUpDataBase;
class UMotionWarpingComponent;

UCLASS()
class PROJECTESCAPE_API AEscapeBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IPawnCombatInterface, public IPawnUIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEscapeBaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	virtual UPawnUIComponent* GetPawnUIComponent() const override;

	virtual void UpdateAimOffset(float DeltaSeconds);

	void NativeGetEscapeAbilitySystemComponent();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UEscapeAbilitySystemComponent* EscapeAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UEscapeAttributeSet* EscapeAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionWarping")
	UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr< UDataAsset_StartUpDataBase> CharacterStartUpData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reference")
	APlayerController* OwnerPlayerController;

	UFUNCTION(Server, Unreliable, BlueprintCallable, Category = "RPCPlaySoundCue")
	void ServerPlaySoundCue(USoundBase* SoundCueToPlayer);

	UFUNCTION(NetMulticast, Unreliable, Category = "RPCPlaySoundCue")
	void MulticastPlaySoundCue(USoundBase* SoundCueToPlayer);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AimOffsetYaw)
	float AimOffsetYaw;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AimOffsetPitch)
	float AimOffsetPitch;

	UFUNCTION()
	void OnRep_AimOffsetYaw();

	UFUNCTION()
	void OnRep_AimOffsetPitch();

public:

	FORCEINLINE UEscapeAbilitySystemComponent* GetEscapeAbilitySystemComponent() const { return EscapeAbilitySystemComponent; }
	FORCEINLINE UEscapeAttributeSet* GetEscapeAttributeSet() const { return EscapeAttributeSet; }
	FORCEINLINE float GetAimOffsetYaw() const { return AimOffsetYaw; }
	FORCEINLINE float GetAimOffsetPitch() const { return AimOffsetPitch; }
};