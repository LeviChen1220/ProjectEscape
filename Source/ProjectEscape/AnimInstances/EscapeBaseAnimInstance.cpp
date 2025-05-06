// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/EscapeBaseAnimInstance.h"
#include "EscapeFunctionLibrary.h"

bool UEscapeBaseAnimInstance::DoesOwnerHasTag(FGameplayTag TagToCheck) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UEscapeFunctionLibrary::NativeDoesActorHaveTag(OwningPawn, TagToCheck);
	}
	return false;
}
