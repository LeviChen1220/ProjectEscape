// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/EscapeBaseAnimInstance.h"
#include "EscapeSurvivorLinkedAnimLayer.generated.h"

class UEscapeSurvivorAnimInstance;
/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UEscapeSurvivorLinkedAnimLayer : public UEscapeBaseAnimInstance
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UEscapeSurvivorAnimInstance* GetSurvivorAnimInstance() const;
};
