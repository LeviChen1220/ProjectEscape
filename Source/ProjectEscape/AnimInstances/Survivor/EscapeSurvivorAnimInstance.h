// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/EscapeCharacterAnimInstance.h"
#include "EscapeSurvivorAnimInstance.generated.h"

class AEscapeSurvivorCharacter;
/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UEscapeSurvivorAnimInstance : public UEscapeCharacterAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	AEscapeSurvivorCharacter* OwningSurvivorCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxStateThreshold = 5.f;

	float IdleElpasedTime;
};
