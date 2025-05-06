// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapeBaseCharacter.h"
//GAS related
#include "AbilitySystem/EscapeAbilitySystemComponent.h"
#include "AbilitySystem/EscapeAttributeSet.h"
//Kismet
#include "Kismet/GameplayStatics.h"
//Animation
#include "MotionWarpingComponent.h"
//Network
#include "Net/UnrealNetwork.h"

// Sets default values
AEscapeBaseCharacter::AEscapeBaseCharacter() :
	AimOffsetYaw(0.f),
	AimOffsetPitch(0.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;
	bReplicates = true;

	EscapeAbilitySystemComponent = CreateDefaultSubobject<UEscapeAbilitySystemComponent>(TEXT("EscapeAbilitySystemComponent"));
	EscapeAttributeSet = CreateDefaultSubobject<UEscapeAttributeSet>(TEXT("EscapeAttributeSet"));
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

UPawnCombatComponent* AEscapeBaseCharacter::GetPawnCombatComponent() const
{
	return nullptr;
}

UPawnUIComponent* AEscapeBaseCharacter::GetPawnUIComponent() const
{
	return nullptr;
}

void AEscapeBaseCharacter::UpdateAimOffset(float DeltaSeconds)
{

}

void AEscapeBaseCharacter::NativeGetEscapeAbilitySystemComponent()
{
	GetAbilitySystemComponent();
}

void AEscapeBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEscapeBaseCharacter, AimOffsetYaw);
	DOREPLIFETIME(AEscapeBaseCharacter, AimOffsetPitch);
}

void AEscapeBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (EscapeAbilitySystemComponent)
	{
		EscapeAbilitySystemComponent->InitAbilityActorInfo(this, this);

		ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Missing CharacterStartUpData to %s"),*GetName());
	}
}

void AEscapeBaseCharacter::OnRep_AimOffsetYaw()
{
}

void AEscapeBaseCharacter::OnRep_AimOffsetPitch()
{
}

UAbilitySystemComponent* AEscapeBaseCharacter::GetAbilitySystemComponent() const
{
	return GetEscapeAbilitySystemComponent();
}

void AEscapeBaseCharacter::ServerPlaySoundCue_Implementation(USoundBase* SoundCueToPlayer)
{
	MulticastPlaySoundCue(SoundCueToPlayer);
}

void AEscapeBaseCharacter::MulticastPlaySoundCue_Implementation(USoundBase* SoundCueToPlayer)
{
	if (SoundCueToPlayer)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SoundCueToPlayer, GetActorLocation());
	}
}
