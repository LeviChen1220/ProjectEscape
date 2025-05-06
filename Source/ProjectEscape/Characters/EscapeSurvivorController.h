// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"

#include "EscapeSurvivorController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API AEscapeSurvivorController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	

public:

	AEscapeSurvivorController();

	virtual FGenericTeamId GetGenericTeamId() const override;

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientAddMappingContext(UInputMappingContext* MappingContext, int32 Priority);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientRemoveMappingContext(UInputMappingContext* MappingContext);

private:

	FGenericTeamId SurvivorTeamID;

	bool bMappingContextAdded;
};
