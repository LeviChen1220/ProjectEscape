// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "PawnUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentChangedDelegate, float, NewPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentAmmunitionAmountChangedDelegate, float, NewCurrentAmmoAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpareAmmunitionAmountChangedDelegate, float, NewSpareAmmoAmount);

class UEscapeWidgetBase;
/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UPawnUIComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnCurrentAmmunitionAmountChangedDelegate OnCurrentAmmunitionAmountChanged;

	UPROPERTY(BlueprintAssignable)

	FOnSpareAmmunitionAmountChangedDelegate OnSpareAmmunitionAmountChanged;

	UPROPERTY(BlueprintReadWrite)
	UEscapeWidgetBase* OwningWidget;

protected:


};
