// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "GameplayTagContainer.h"
#include "EscapeTypes/EscapeStructTypes.h"
#include "DataAsset_SurvivorStartUpData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UDataAsset_SurvivorStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()
public:

	virtual void GiveToAbilitySystemComponent(UEscapeAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (TitleProperty = "InputTag"))
	TArray< FEscapeSurvivorAbilitySet> SurvivorStartUpAbilitySets;
};
