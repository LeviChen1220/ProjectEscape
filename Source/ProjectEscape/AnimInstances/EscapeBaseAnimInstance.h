// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"

#include "EscapeBaseAnimInstance.generated.h"
 
/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UEscapeBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool DoesOwnerHasTag(FGameplayTag TagToCheck) const;
};
