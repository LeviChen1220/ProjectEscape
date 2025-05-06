// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/EscapeBaseCharacter.h"
#include "EscapeEnemyCharacter.generated.h"

class UEnemyCombatComponent;
class UEnemyUIComponent;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API AEscapeEnemyCharacter : public AEscapeBaseCharacter
{
	GENERATED_BODY()
	
public:

	AEscapeEnemyCharacter();

	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;

	virtual UPawnUIComponent* GetPawnUIComponent() const override;

	virtual UEnemyUIComponent* GetEnemyUIComponent() const override;

	UFUNCTION(Server, Unreliable, BlueprintCallable, Category = "Material")
	void Server_ScalarParameterValueForMaterial(FName ParameterName, float InValue);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_EnemySendGameplayEventToActor(AActor* InActor, FGameplayTag TagToSend, FGameplayEventData InEventData);

protected:

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UEnemyCombatComponent* EnemyCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UEnemyUIComponent* EnemyUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* EnemyHealthWidgetComponent;

private:

	void InitEnemyStartUpData();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_ScalarParameterValueForMaterial(FName ParameterName, float InValue);

public:

	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }
};
