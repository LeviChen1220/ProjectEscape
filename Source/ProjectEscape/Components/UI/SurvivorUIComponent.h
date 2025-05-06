// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "SurvivorUIComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API USurvivorUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()
	
public:

	USurvivorUIComponent();
	//UPROPERTY(BlueprintAssignable)

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_PlayFadeHUDAnim(bool bShouldPlayInReverse);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bFadeHUDAnimHasPlayed;
};
