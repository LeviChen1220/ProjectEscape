// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EscapeTypes/EscapeEnumTypes.h"
#include "EscapeFunctionLibrary.generated.h"


class UEscapeAbilitySystemComponent;
class UPawnCombatComponent;
class AEscapeBaseCharacter;
class UEscapeAttributeSet;
/**
 * 
 */


UCLASS()
class PROJECTESCAPE_API UEscapeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public: 

	//cpp
	static UEscapeAbilitySystemComponent* NativeGetEscapeASCFromActor(AActor* InActor);
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	//BP
	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static void RemoveGameplayTagToActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToRemove, EEscapeConfirmType& OutConfirmType);

	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary", meta = (DisplayName = "Get PawnCombatComponent From Actor", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor * InActor, EEscapeValidType& OutValidType);

	UFUNCTION(BlueprintPure, Category = "FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);

	UFUNCTION(BlueprintPure, Category = "FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference);

	static AEscapeBaseCharacter* NativeGetEscapeBaseCharacterFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary", meta = (DisplayName = "Get GetEscapeBaseCharacter From Actor", ExpandEnumAsExecs = "OutValidType"))
	static AEscapeBaseCharacter* BP_GetEscapeBaseCharacterFromActor(AActor* InActor, EEscapeValidType& OutValidType);

	UFUNCTION(BlueprintPure, Category = "FunctionLibrary", meta = (DisplayName = "Check Is Actor Dead"))
	static bool BP_CheckIsActorDead(AActor* InActor);

};
