// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Survivor/EscapeSurvivorAnimInstance.h"
#include "Characters/EscapeSurvivorCharacter.h"


void UEscapeSurvivorAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningCharacter)
	{
		OwningSurvivorCharacter = Cast<AEscapeSurvivorCharacter>(OwningCharacter);
	}
}

void UEscapeSurvivorAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (bHasAcceleration)
	{
		IdleElpasedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		IdleElpasedTime += DeltaSeconds;
		bShouldEnterRelaxState = (IdleElpasedTime >= EnterRelaxStateThreshold);
	}
}
