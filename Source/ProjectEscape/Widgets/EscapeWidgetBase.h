// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EscapeWidgetBase.generated.h"

class USurvivorUIComponent;
class UEnemyUIComponent;
/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UEscapeWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Owning SurvivorUIComponent Initialized"))
	void BP_OnOwningSurvivorUIComponentInitialized(USurvivorUIComponent* OwningSurvivorUIComponent);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Owning EnemyUIComponent Initialized"))
	void BP_OnOwningEnemyIComponentInitialized(UEnemyUIComponent* OwningEnemyUIComponent);
	

public:

	UFUNCTION(BlueprintCallable)
	void InitEnemyCreatedWidget(AActor* OwningEnemyActor);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_PlayFadeAnim(bool bShouldPlayInReverse);
};
